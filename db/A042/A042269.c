/*
 * A042269  Denominators of continued fraction convergents to sqrt(660).
 * https://oeis.org/A042269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2158*a(n-6) - a(n-12) for n > 11 with initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=13, a(4)=29, a(5)=42, a(6)=2129,
 *            a(7)=2171, a(8)=6471, a(9)=28055, a(10)=62581, a(11)=90636
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoisation cache */
    static size_t size = 0;          /* number of elements currently cached */

    /* initialise cache with first 12 terms */
    if (cache == NULL) {
        size = 12;
        cache = (seq_t*)malloc(size * sizeof(seq_t));
        static const seq_t init[12] = {
            1,   1,   3,   13,  29,  42,
            2129,2171,6471,28055,62581,90636
        };
        for (size_t i = 0; i < 12; ++i) {
            cache[i] = init[i];
        }
    }

    /* grow cache and compute any missing terms up to n */
    if ((size_t)n + 1 > size) {
        size_t newcap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));

        for (size_t i = size; i <= (size_t)n; ++i) {
            seq_t t = (seq_t)2158 * cache[i - 6];
            t -= cache[i - 12];
            cache[i] = t;
        }
        size = newcap;
    }

    return cache[(size_t)n];
}
