/*
 * A024395  a(n) = n-th elementary symmetric function of the first n+1
 *          positive integers congruent to 2 mod 3.
 * https://oeis.org/A024395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A024395: elementary symmetric function of first n+1 integers ≡2 (mod 3)
    a(0) = 1
    a(1) = 7
    a(n+1) = (6*n + 7) * a(n) - (3*n + 2)^2 * a(n-1)   for n ≥ 1
*/

#include <stdlib.h>

/* static cache of previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* extend cache up to n */
    if (n >= cache_len) {
        /* enlarge array */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) /* allocation failed, return 0 as fallback */
            return (seq_t)0;
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)7;
            } else {
                idx_t m = i - 1;                     /* m = n in recurrence */
                seq_t factor1 = (seq_t)(6 * m + 7);   /* (6*n + 7)  */
                seq_t factor2 = (seq_t)(3 * m + 2);   /* (3*n + 2)  */
                seq_t term1 = factor1 * cache[i - 1];
                seq_t sq = factor2 * factor2;
                seq_t term2 = sq * cache[i - 2];
                cache[i] = term1 - term2;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
