/*
 * A061560  Lengths of Farey-series (A005728) which are not primes.
 * https://oeis.org/A061560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005728(A055197(n-1)), a(1) = 1 A005728(k) = 1 +
 *            Σ_{i=1..k} φ(i) (length of Farey series of order k) A055197(m) =
 *            index of the m‑th non‑prime Farey length (counting 1 at index 0)
 *            So a(n) is the Farey length (value of A005728) at the (n‑1)‑th
 *            non‑prime length index. The routine computes φ a...
 * Range:     u8 n <= 11, u16 n <= 353, u32 n <= 777, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* ---------- static scratch space ---------- */
static idx_t cur_limit = 0;           /* last computed limit for φ, cum, indices */
static idx_t *phi = NULL;             /* φ(i) for 0 ≤ i ≤ cur_limit                    */
static idx_t *cum = NULL;             /* cumulative farey lengths F(i)                 */
static idx_t *nonprime_indices = NULL;/* indices i where F(i) is NOT prime             */
static size_t nonprime_count = 0;     /* number of stored indices                      */
static size_t nonprime_cap   = 0;     /* capacity of nonprime_indices array            */

/* Helper to realloc an idx_t array with C‑style cast */
static void *xmalloc(size_t sz) { return (void*)malloc(sz); }
static void *xrealloc(void *p, size_t sz) { return (void*)realloc(p, sz); }

/* Rebuild φ, cumulative Farey sums and the list of non‑prime indices
   up to (and including) new_limit.  This is called when we need more
   data than we currently have.                                */
static void recompute_up_to(idx_t new_limit)
{
    /* Allocate φ array (uint64_t compatible) */
    idx_t *new_phi = (idx_t*)xmalloc((new_limit + 1) * sizeof(idx_t));
    if (!new_phi) return; /* out of memory; caller will abort */

    /* initialise φ(i) = i; φ(0)=0, φ(1)=1 */
    for (idx_t i = 0; i <= new_limit; ++i) new_phi[i] = i;
    new_phi[0] = 0;
    if (new_limit >= 1) new_phi[1] = 1;

    /* Sieve φ: for every prime p subtract φ(p*k)/p */
    for (idx_t p = 2; p <= new_limit; ++p) {
        if (new_phi[p] == p) { /* p is prime */
            for (idx_t j = p; j <= new_limit; j += p) {
                new_phi[j] -= new_phi[j] / p;
            }
        }
    }

    /* Allocate cumulative sum array */
    idx_t *new_cum = (idx_t*)xmalloc((new_limit + 1) * sizeof(idx_t));
    /* Count of non‑prime indices + capacity */
    size_t new_cap = 64;
    idx_t *new_indices = (idx_t*)xmalloc(new_cap * sizeof(idx_t));
    size_t new_cnt = 0;

    /* F(0) = 1, treat separately */
    new_cum[0] = (idx_t)1;
    /* 1 is not prime */
    new_indices[new_cnt++] = 0;

    /* Build cum[i] = cum[i-1] + φ(i) for i>=1, and collect non‑prime indices */
    for (idx_t i = 1; i <= new_limit; ++i) {
        new_cum[i] = new_cum[i-1] + new_phi[i];
        if (!is_prime_big((seq_t)new_cum[i])) {
            if (new_cnt == new_cap) {          /* extend indices array */
                new_cap <<= 1;
                new_indices = (idx_t*)xrealloc(new_indices, new_cap * sizeof(idx_t));
            }
            new_indices[new_cnt++] = i;
        }
    }

    /* Replace old tables with the newly computed ones. */
    free(phi);
    free(cum);
    free(nonprime_indices);
    phi = new_phi;
    cum = new_cum;
    nonprime_indices = new_indices;
    nonprime_count = new_cnt;
    nonprime_cap   = new_cap;
    cur_limit = new_limit;
}

/* Ensure that we have at least 'needed' elements in nonprime_indices. */
static void ensure_at_least(size_t needed)
{
    if (nonprime_count >= needed) return;

    idx_t limit = cur_limit > 0 ? cur_limit << 1 : 10;
    while (nonprime_count < needed) {
        recompute_up_to(limit);
        if (nonprime_count >= needed) break;
        limit <<= 1;
        if (limit == 0) break; /* overflow protection */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) = F(0) = 1 */
    if (n == 1) return (seq_t)1;

    /* For n>1 we need the (n‑1)‑th non‑prime Farey index. */
    size_t target = (size_t)n;              /* we need at least n indices (0‑based) */
    ensure_at_least(target);

    idx_t idx = nonprime_indices[n - 1];
    return (seq_t)cum[idx];                  /* cast to seq_t for arbitrary‑precision builds */
}
