/*
 * A188697  Expansion of (1+2*x^2)/(1-26*x+2*x^2-52*x^3+4*x^4).
 * https://oeis.org/A188697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=26, a(2)=676, a(3)=17576;
 *            a(n)=26*a(n-1)-2*a(n-2)+52*a(n-3)-4*a(n-4) for n>=4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t maxn = 0;  /* number of values stored (cache[0..maxn-1]) */

    size_t idx = (size_t)n;        /* index into cache */
    if (idx + 1 > cap) {
        cache = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
        cap   = idx + 1;
    }

    if (idx < maxn)                /* already computed */
        return cache[idx];

    /* Compute from maxn to idx inclusive */
    for (size_t i = maxn; i <= idx; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)26;
        else if (i == 2)
            cache[2] = (seq_t)676;
        else if (i == 3)
            cache[3] = (seq_t)17576;
        else
            cache[i] = (seq_t)26 * cache[i-1]
                     - (seq_t)2 * cache[i-2]
                     + (seq_t)52 * cache[i-3]
                     - (seq_t)4 * cache[i-4];
    }

    maxn = idx + 1;
    return cache[idx];
}
