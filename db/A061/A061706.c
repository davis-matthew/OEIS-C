/*
 * A061706  G.f.:
 *          x^2*(x-1)^8/(4*x^9+x^8-79*x^7+166*x^6-225*x^5+206*x^4-129*x^3+50*x^2-11*x+1).
 * https://oeis.org/A061706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 50*a(n-2) + 129*a(n-3) - 206*a(n-4) +
 *            225*a(n-5) - 166*a(n-6) + 79*a(n-7) - a(n-8) - 4*a(n-9) for n >=
 *            11, with a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=11, a(5)=44,
 *            a(6)=185, a(7)=805, a(8)=3552, a(9)=15671, a(10)=68827
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static idx_t  len   = 0;
    if (!cache) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));

        /* base values 0..10 */
        cache[0] = 0;   cache[1] = 0;   cache[2] = 1;
        cache[3] = 3;   cache[4] = 11;  cache[5] = 44;
        cache[6] = 185; cache[7] = 805; cache[8] = 3552;
        cache[9] = 15671; cache[10] = 68827;
        len = 11;
    }

    /* return cached value if already known */
    if (n < len) return cache[n];

    /* recurrence coefficients */
    const int coeffs[9] = { 11, -50, 129, -206, 225, -166, 79, -1, -4 };

    /* compute missing terms up to n */
    while (len <= n) {
        if (len == cap) {
            cap <<= 1;                           /* double capacity */
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }

        __int128 sum = 0;
        for (int j = 0; j < 9; ++j) {
            sum += (__int128)coeffs[j] *
                    (__int128)cache[(size_t)(len - 1 - j)];
        }
        cache[len] = (seq_t)sum;                 /* final value fits in seq_t */
        ++len;
    }

    return cache[n];
}
