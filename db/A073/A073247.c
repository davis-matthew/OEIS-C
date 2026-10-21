/*
 * A073247  Squarefree numbers k such that k-1 and k+1 are not squarefree.
 * https://oeis.org/A073247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5773, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n-th squarefree integer k such that k‑1 and k+1 are NOT squarefree. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* External linkage as required by the build. */
seq_t a(idx_t n);

/* -----------------  Global state  ----------------- */

/* Cached sequence terms */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;

/* Square‑free marker array (true if the index is squarefree). */
static bool *squarefree = NULL;
static size_t sieve_limit = 0;     /* largest index for which squarefree[] is valid */

/* -----------------  Helper functions  ----------------- */

static void push_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return;                 /* allocation failure: silently abort */
        terms     = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Extend the sieve and rebuild terms for the newly covered numbers. */
static void extend_limit(size_t new_limit)
{
    if (new_limit <= sieve_limit) return;      /* already covered */

    size_t old_limit = sieve_limit;            /* last processed upper bound */

    /* Reallocate the square‑free array. */
    bool *new_sf = (bool*)realloc(squarefree, (new_limit + 2) * sizeof(bool));
    if (!new_sf) return;
    squarefree = new_sf;

    /* Mark new entries as true (squarefree) initially. */
    for (size_t i = old_limit + 1; i <= new_limit + 1; ++i) squarefree[i] = true;

    /* Generate primes up to sqrt(new_limit+1) and use them to mark multiples of squares. */
    size_t sqrt_limit = (size_t)sqrt((double)(new_limit + 1)) + 1;
    char *is_comp = (char*)calloc(sqrt_limit + 1, 1);    /* 0 = prime, 1 = composite */
    if (!is_comp) return;

    for (size_t i = 2; i * i <= sqrt_limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= sqrt_limit; j += i)
                is_comp[j] = 1;
        }
    }

    for (size_t p = 2; p <= sqrt_limit; ++p) {
        if (!is_comp[p]) {
            size_t pp = p * p;
            if (pp > new_limit + 1) continue;
            for (size_t j = pp; j <= new_limit + 1; j += pp)
                squarefree[j] = false;
        }
    }
    free(is_comp);

    /* Recompute terms for the newly covered interval. */
    size_t start_k = (old_limit >= 2) ? old_limit + 1 : 2;   /* ensure k >= 2 */
    size_t end_k   = new_limit;                              /* include k = new_limit */

    for (size_t k = start_k; k <= end_k; ++k) {
        if (squarefree[k] && !squarefree[k - 1] && !squarefree[k + 1])
            push_term((seq_t)k);
    }

    sieve_limit = new_limit;
}

/* Ensure the cache contains at least n terms.  Uses the 11‑<a(n)<12n
 * heuristic from OEIS comments and enlarges the sieve as needed. */
static void ensure_terms(size_t n)
{
    if (terms_len >= n) return;

    size_t required = 12 * n + 200;
    if (required < 200) required = 200;

    extend_limit(required);
    while (terms_len < n) {
        required *= 2;
        extend_limit(required);
    }
}

/* -----------------  Public interface  ----------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out‑of‑range guard, though offset is 1 */

    ensure_terms((size_t)n);
    return terms[n - 1];
}
