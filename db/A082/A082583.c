/*
 * A082583  Quotients C(p, k)/p sorted, with duplicates removed, p being
 *          prime.
 * https://oeis.org/A082583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest distinct integer of the form C(p,k)/p,
 *            where p is a prime and 1 ≤ k ≤ p‑1
 * Range:     u8 none, u16 n <= 3, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   append_candidate, build_primes, cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"
#include "../utils/build_primes.h"
#include "../utils/cmp_seq_t.h"

/* static cache */
static seq_t *cached_values = NULL;
static idx_t cached_len = 0;
static bool initialized = false;

/* compute C(p,k)/p as C(p-1,k-1)/k */
static seq_t compute_quotient(idx_t p, idx_t k)
{
    if (k >= p) return 0;
    if (k == 1) return 1;                 /* not used, but for completeness */

    idx_t r = k - 1;
    unsigned __int128 num = 1;
    for (idx_t i = 0; i < r; ++i)
        num *= (unsigned __int128)(p - 1 - i);

    unsigned __int128 denom = 1;
    for (idx_t i = 2; i <= k; ++i)
        denom *= (unsigned __int128)i;

    return (seq_t)(num / denom);
}

/* generate all needed terms once */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Parameters chosen so that at least 25 000 terms are produced,
       far more than the 10 000 required by the problem statement. */
    const idx_t PRIME_LIMIT   = 400000;     /* generate primes up to this limit           */
    const seq_t VALUE_LIMIT   = 2000000;    /* ignore any quotient larger than this value */

    /* obtain primes up to PRIME_LIMIT */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(PRIME_LIMIT, &primes, &prime_cnt);

    /* temporary dynamic array for all quotients (before deduplication) */
    size_t len  = 0;
    size_t cap  = 0;
    seq_t *vals = NULL;

    /* generate quotients for each prime */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];

        /* k starts at 2 (k=1 gives the value 1, already produced by p=3) */
        for (idx_t k = 2; k < p; ++k) {
            seq_t q = compute_quotient(p, k);
            if (q > VALUE_LIMIT) break;          /* further k will only increase q */
            append_candidate(&vals, &len, &cap, q);
        }
    }

    free(primes);   /* primes were allocated by build_primes */

    /* sort and deduplicate */
    if (len > 0) {
        qsort(vals, len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (uniq == 0 || vals[i] != vals[uniq - 1]) {
                vals[uniq++] = vals[i];
            }
        }
        /* shrink storage */
        seq_t *tmp = (seq_t *)realloc(vals, uniq * sizeof(seq_t));
        cached_values = tmp ? tmp : vals;
        cached_len    = uniq;
    } else {
        cached_values = NULL;
        cached_len    = 0;
    }
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > cached_len) {
        /* out‑of‑range access; return 0 as a safe default */
        return 0;
    }
    return cached_values[n - 1];
}
