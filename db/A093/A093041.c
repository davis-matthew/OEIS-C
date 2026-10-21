/*
 * A093041  Expansion of (1-4x+6x^2-3x^3)/(1-5x+9x^2-8x^3+4x^4).
 * https://oeis.org/A093041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=6;
   a(n)=5*a(n-1)-9*a(n-2)+8*a(n-3)-4*a(n-4)   for n>=4   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    static seq_t *cache = NULL;    /* memoised values */
    static idx_t cache_size = 0;   /* allocated size */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_size = 4;                   /* we know a[0..3] */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
    }

    /* Extend the cache if needed and compute missing values */
    if (n >= cache_size) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = 5 * cache[i-1] - 9 * cache[i-2] + 8 * cache[i-3] - 4 * cache[i-4];
        cache_size = n + 1;
    }

    return cache[n];
}
