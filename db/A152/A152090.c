/*
 * A152090  a(n) = 2^n*Product_{k=1..floor((n-1)/2)} (1 + 2*cos(k*Pi/n)^2 +
 *          4*cos(k*Pi/n)^4).
 * https://oeis.org/A152090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,a(1)=1,a(2)=1,a(3)=3,a(4)=7;
   a(n)=a(n-1)+3*a(n-2)+a(n-3)-a(n-4) for n>=5 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size == 0) {
        /* initialise cache with at least 5 entries */
        cache_size = (n < 5 ? 5 : n + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory: return 0 */

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 3;
        cache[4] = 7;

        for (idx_t i = 5; i < cache_size; ++i) {
            cache[i] = cache[i-1] + 3*cache[i-2] + cache[i-3] - cache[i-4];
        }
    }

    if ((idx_t)n >= cache_size) {
        /* extend cache to cover index n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                     /* out of memory: return 0 */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = cache[i-1] + 3*cache[i-2] + cache[i-3] - cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
