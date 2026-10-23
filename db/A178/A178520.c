/*
 * A178520  Number of ordered trees with n edges and with no vertex of
 *          outdegree 2 that have two leaves as their two children.
 * https://oeis.org/A178520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + Sum_{k=0..n-1} a(k) * a(n-1-k) Initial values:
 *            a(0)=a(1)=a(2)=1 for n>=0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of currently stored elements */

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;

        /* Initialise the missing base cases (a(0)=a(1)=a(2)=1) */
        for (size_t i = cache_len; i < 3 && i < new_len; ++i)
            cache[i] = (seq_t)1;

        /* Compute values from the old length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if ((idx_t)i < 3) {          /* base case already set */
                continue;
            }
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                sum += cache[k] * cache[i - 1 - k];
            }
            cache[i] = cache[i - 3] + sum;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
