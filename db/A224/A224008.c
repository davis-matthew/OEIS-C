/*
 * A224008  Number of n X 4 0..2 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 58*a(n-2) + 105*a(n-3) + 20*a(n-4) * -
 *            183*a(n-5) + 220*a(n-6) + 240*a(n-7) * Initial values: a(1)=15,
 *            a(2)=225, a(3)=2661, a(4)=28541, * a(5)=297859, a(6)=3094127,
 *            a(7)=32148473
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)          /* undefined for n<1, return 0 */
        return 0;

    /* initialise cache with the first 7 terms */
    if (cache == NULL) {
        cache_size = 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache)   /* allocation failure */
            return 0;
        cache[0] = (seq_t)15;
        cache[1] = (seq_t)225;
        cache[2] = (seq_t)2661;
        cache[3] = (seq_t)28541;
        cache[4] = (seq_t)297859;
        cache[5] = (seq_t)3094127;
        cache[6] = (seq_t)32148473;
    }

    if (n <= cache_size)          /* cached value is ready */
        return cache[n-1];

    /* extend cache up to n using the recurrence */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache)          /* allocation failure */
            return 0;
        cache = new_cache;
        for (idx_t i = cache_size; i < n; ++i) {
            cache[i] =
                (seq_t)15  * cache[i-1]
              + (seq_t)105 * cache[i-3]
              + (seq_t)20  * cache[i-4]
              + (seq_t)220 * cache[i-6]
              + (seq_t)240 * cache[i-7]
              - (seq_t)58  * cache[i-2]
              - (seq_t)183 * cache[i-5];
        }
        cache_size = n;
    }

    return cache[n-1];
}
