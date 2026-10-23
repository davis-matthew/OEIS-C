/*
 * A025234  An L-tile is a 2 X 2 square with the upper 1 X 1 subsquare
 *          removed; no rotations are allowed. a(n) = number of tilings of a 4
 *          X n rectangle using tiles that are either 1 X 1 squares or
 *          L-tiles.
 * https://oeis.org/A025234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+5*a(n-2)+4*a(n-3)-a(n-5) (n>=5) * a(0)=1, a(1)=0,
 *            a(2)=4, a(3)=8, a(4)=28
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 4;
        cache[3] = 8;
        cache[4] = 28;
    }

    if (n < cache_len)
        return cache[n];

    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-1];
        val += 5 * cache[i-2];
        val += 4 * cache[i-3];
        val -= cache[i-5];
        cache[i] = val;
    }
    cache_len = new_len;
    return cache[n];
}
