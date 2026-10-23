/*
 * A103610  4 X infinity array read by rows: let M = {{0, 1, 0, 0}, {0, 0, 1,
 *          0}, {0, 0, 0, 1}, {1, 0, 1, 0}}, w[0] = {0, 0, 1, 1}', w[n]' =
 *          M*w[n -1]'; the n-th row of the array is w[n]' (the prime denotes
 *          transpose).
 * https://oeis.org/A103610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A103610: a(n) = a(n-8) + a(n-16) for n > 15
 * Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=0, a(5)=1,
 *                 a(6)=1, a(7)=1, a(8)=1, a(9)=1, a(10)=1, a(11)=1,
 *                 a(12)=1, a(13)=1, a(14)=1, a(15)=2
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init_vals[16] = {
        0, 0, 1, 1,
        0, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 2
    };

    /* initialize cache with first 16 entries on first call */
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* extend cache to include up to n */
    idx_t new_size = n + 1;
    if (new_size > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) /* allocation failure: safe fallback */
            return 0;
        cache = new_cache;

        /* compute new entries using recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            /* recurrence applies for i >= 16 */
            seq_t v1 = cache[i - 8];
            seq_t v2 = cache[i - 16];
            cache[i] = v1 + v2;
        }
        cache_size = new_size;
    }

    return cache[n];
}
