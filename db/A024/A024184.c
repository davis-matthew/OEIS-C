/*
 * A024184  Third elementary symmetric function of 3,4,...,n+4.
 * https://oeis.org/A024184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+7)*(n^2+13*n+46)/48
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 73, u64 n <= 3094,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Index offset: 1  (a(1) = 60) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t fac[5];

    fac[0] = N;                       // n
    fac[1] = N + 1;                   // n+1
    fac[2] = N + 2;                   // n+2
    fac[3] = N + 7;                   // n+7

    seq_t NN = N * N;                 // n^2
    fac[4] = NN + (seq_t)13 * N + (seq_t)46; /* n^2 + 13*n + 46 */

    seq_t denom = (seq_t)48;          // divisor
    seq_t res   = (seq_t)1;

    for (int i = 0; i < 5; ++i) {
        seq_t g = gcd(fac[i], denom);
        if (g > (seq_t)1) {
            fac[i] = fac[i] / g;
            denom  = denom  / g;
        }
        res = res * fac[i];
    }
    if (denom != (seq_t)1) {
        res = res / denom;            /* should be 1 */
    }

    return res;
}
