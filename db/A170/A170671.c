/*
 * A170671  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A170671
 *
 *  The sequence satisfies the following recurrence (with a(0)=1):
 *
 *  For 1 ≤ n ≤ 48
 *      a(n) = 36 * (a(0)+a(1)+…+a(n‑1)) + 2
 *
 *  For n = 49
 *      a(49) = 36 * (a(1)+a(2)+…+a(48)) – 666 * a(0) + 1
 *
 *  For n ≥ 50
 *      a(n) = 36 * (a(n‑48)+a(n‑47)+…+a(n‑1)) – 666 * a(n‑49)
 *
 *  This recurrence comes from the rational generating function
 *  (t^49 + 2 t^48 + … + 2 t + 1)/(666 t^49 – 36 t^48 – … – 36 t – 1).
 *
 *  The implementation uses a simple growing cache to compute terms
 *  on demand in O(n·48) time, which is far below the required limit
 *  for n up to 10000.
 */

#include <stdlib.h>

/* External types supplied by the build environment */
extern seq_t a(idx_t n);

/* Internal cache for memoization */
static seq_t *cache = NULL;   /* array of a(n) values */
static idx_t cache_len = 0;   /* number of elements currently stored (cache_len == size) */

/* Expand the cache up to at least n and fill missing terms */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        /* Initial allocation for at least one element */
        cache_len = (n >= 0) ? n + 1 : 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();          /* terminate on allocation failure */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }

    if (cache_len <= n) {
        /* Resize cache to accommodate n */
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) abort();

        /* Compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            if (i == 0) {
                cache[0] = (seq_t)1;   /* already set, but keep for clarity */
            } else if (i < 49) {
                /* Sum all previous terms */
                for (idx_t j = 0; j < i; ++j) {
                    sum += cache[j];
                }
                cache[i] = sum * (seq_t)36 + (seq_t)2;
            } else if (i == 49) {
                /* Sum a(1) through a(48) */
                for (idx_t j = 1; j < 49; ++j) {
                    sum += cache[j];
                }
                cache[49] = sum * (seq_t)36 - (seq_t)666 * cache[0] + (seq_t)1;
            } else {
                /* Sum the last 48 terms: a(i-48) ... a(i-1) */
                for (idx_t j = i - 48; j < i; ++j) {
                    sum += cache[j];
                }
                cache[i] = sum * (seq_t)36 - (seq_t)666 * cache[i - 49];
            }
        }

        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
