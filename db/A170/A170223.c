/*
 * A170223  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170223: Number of reduced words of length n in a Coxeter group.
   Recurrence (for all n ≥ 0):
      a(n) = c(n) + 20 · Σ_{k=1}^{39} a(n‑k) – 210 · a(n‑40),
   where
      c(0) = 1,
      1 ≤ k ≤ 39 : c(k) = 2,
      c(40) = 1,
      otherwise 0.
   Initial values a(0) … a(15) are supplied by the recurrence. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of elements currently stored */

    /* Grow the cache to hold indices 0 … n */
    if (n + 1 > cache_len) {
        /* reallocate with the new size */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – terminate (sequence is undefined) */
            exit(1);
        }
        cache = tmp;
        /* initialise the newly allocated portion to zero */
        if (cache_len < n + 1)
            memset(&cache[cache_len], 0, (size_t)(n + 1 - cache_len) * sizeof(seq_t));
        cache_len = (size_t)(n + 1);
    }

    /* Compute any terms that haven't been calculated yet */
    for (idx_t idx = 0; idx < n + 1; ++idx) {
        /* Skip if already computed */
        if (idx < cache_len && (idx < cache_len && idx < cache_len && cache[idx] != 0))
            continue;

        /* Compute c(idx) */
        seq_t c = 0;
        if (idx == 0)          c = (seq_t)1;
        else if (idx <= 39)    c = (seq_t)2;
        else if (idx == 40)    c = (seq_t)1;

        /* Sum of the previous up to 39 terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 39 && idx >= k; ++k)
            sum += cache[idx - k];

        seq_t term = c + (seq_t)20 * sum;
        if (idx >= 40)
            term -= (seq_t)210 * cache[idx - 40];

        cache[idx] = term;
    }

    return cache[n];
}
