/*
 * A169188  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n ≥ 1: a(n) = 29 * (a(n‑1)+…+a(n‑26)) + 435 *
 *            a(n‑27) (if n ≥ 27) + b(n) where b(n) = 2 for 1 ≤ n ≤ 26, b(27)
 *            = 1, otherwise 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#  include <algorithm>
#endif

/* Authoritative cache for previously computed terms */
static seq_t *cache   = NULL;  /* array of a(0)…a(max_n) */
static idx_t  cache_cap  = 0;  /* current allocation size */
static idx_t  cache_last = 0;  /* next index to compute */

/* Ensure the cache has space for indices up to n */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    /* copy old contents */
    if (cache) {
        std::copy(cache, cache + cache_cap, new_cache);
        delete [] cache;
    }
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
#endif
    cache    = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* base case */

    ensure_cache(n);

    /* initialise a(0) once */
    if (cache_last == 0) {
        cache[0] = (seq_t)1;
        cache_last = 1;
    }

    /* compute terms incrementally */
    for (idx_t i = cache_last; i <= n; ++i) {
        seq_t sum = 0;
        /* sum of the last 26 values */
        for (idx_t k = 1; k <= 26 && i >= k; ++k) {
            sum += cache[i - k];
        }
        /* contribution of a(i‑27) */
        seq_t term27 = 0;
        if (i >= 27) {
            term27 = cache[i - 27] * (seq_t)435;
        }

        /* b(i) depending on index */
        seq_t bn = 0;
        if (i <= 26) {
            bn = (seq_t)2;
        } else if (i == 27) {
            bn = (seq_t)1;
        }

        /* recurrence */
        cache[i] = term27 + (seq_t)29 * sum + bn;
    }

    cache_last = n + 1;
    return cache[n];
}
