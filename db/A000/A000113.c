/*
 * A000113  Number of transformation groups of order n.
 * https://oeis.org/A000113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(2^e) = 3*2^floor((e-1)/2) for 0<e<7,
 *            a(2^e) = 3*2^(e-4) for e>=7, a(3^e) = 4 for 0<e<3, a(3^e) =
 *            4*3^(e-2) for e>=3, and a(p^e) = (p+1)*p^(e-1) for p>3
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t m = n;
    int e, i;

    e = 0;
    while (m % 2 == 0) { m /= 2; e++; }
    if (e > 0) {
        int k = (e < 7) ? (e - 1) / 2 : e - 4;
        seq_t t = 3;
        for (i = 0; i < k; i++) t *= 2;
        r *= t;
    }

    e = 0;
    while (m % 3 == 0) { m /= 3; e++; }
    if (e > 0) {
        seq_t t = 4;
        for (i = 2; i < e; i++) t *= 3;
        r *= t;
    }

    {
        idx_t p;
        for (p = 5; p <= m / p; p += 2) {
            if (m % p == 0) {
                seq_t t;
                m /= p;
                t = (seq_t)p + 1;
                while (m % p == 0) { m /= p; t *= (seq_t)p; }
                r *= t;
            }
        }
        if (m > 1) r *= (seq_t)m + 1;
    }
    return r;
}
