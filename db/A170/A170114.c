/*
 * A170114  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 37: a(n) = 2 + 7 * (a(0) + a(1) + … +
 *            a(n-1)) a(38) = 1 + 7 * (a(1) + a(2) + … + a(37)) – 28 * a(0)
 *            For n > 38: a(n) = 7 * (a(n-1) + a(n-2) + … + a(n-37)) – 28 *
 *            a(n-38) (The recurrence follows from the rational generating
 *            function) The function memoizes values so that calls...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* internal cache for memoization */
static seq_t *cache = NULL;     /* pointer to stored terms */
static idx_t cache_len = 0;     /* number of terms currently cached */

/* Ensure cache holds terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;        /* already enough */

    /* allocate new size */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return;           /* allocation failed, leave cache unchanged */

    cache = new_cache;

    /* compute missing terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;                 /* a(0) */
        } else if (i <= 37) {
            /* a(i) = 2 + 7 * sum_{k=0}^{i-1} a(k) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                sum += (seq_t)7 * cache[k];
            }
            cache[i] = (seq_t)2 + sum;
        } else if (i == 38) {
            /* a(38) = 1 + 7 * sum_{k=1}^{37} a(k) – 28 * a(0) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 37; ++k) {
                sum += (seq_t)7 * cache[k];
            }
            cache[i] = (seq_t)1 + sum - (seq_t)28 * cache[0];
        } else {
            /* recurrence for n > 38:
               a(n) = 7 * sum_{i=1}^{37} a(n-i) – 28 * a(n-38) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 37; ++k) {
                sum += (seq_t)7 * cache[i - k];
            }
            cache[i] = sum - (seq_t)28 * cache[i - 38];
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
