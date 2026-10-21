/*
 * A205191  Number of (n+1) X 7 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock differing from each horizontal
 *          or vertical neighbor.
 * https://oeis.org/A205191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)+a(n-6) for n>10
 *            Initial values: a(1)=216, a(2)=272, a(3)=384, a(4)=576,
 *            a(5)=896, a(6)=1408, a(7)=2048, a(8)=2944, a(9)=4224, a(10)=6144
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 45, u64 n <= 103, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;
    static size_t computed_max = 0;

    size_t needed = (size_t)n + 1;           /* indices 0..n */
    if (cache_alloc < needed) {
        size_t new_alloc = needed > 16 ? needed * 2 : 16;
        seq_t *new_cache = (seq_t *)malloc(new_alloc * sizeof(seq_t));
        if (!new_cache) abort();
        memset(new_cache, 0, new_alloc * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_alloc * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_alloc = new_alloc;
    }

    /* initialise first ten terms once */
    if (cache[1] == 0) {
        cache[1] = 216;
        cache[2] = 272;
        cache[3] = 384;
        cache[4] = 576;
        cache[5] = 896;
        cache[6] = 1408;
        cache[7] = 2048;
        cache[8] = 2944;
        cache[9] = 4224;
        cache[10] = 6144;
    }

    if (computed_max < 10) computed_max = 10;

    if (computed_max < (size_t)n) {
        for (idx_t i = (idx_t)(computed_max + 1); i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] - cache[i-3]
                       + cache[i-4] - cache[i-5] + cache[i-6];
        }
        computed_max = (size_t)n;
    }

    return cache[n];
}
