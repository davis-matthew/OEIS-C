/*
 * A081284  An interleaved sequence of pyramidal and polygonal numbers.
 * https://oeis.org/A081284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+1-(-1)^n)*(2*n+5-(-1)^n)*(2*n+9-(-1)^n)*
 *            (2*n^2+15*n+51+3*(n-5)*(-1)^n) / 30720
 * Range:     u8 n <= 10, u16 n <= 38, u32 n <= 380, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_ull.h"

seq_t a(idx_t n)
{
    const unsigned long long D0 = 30720;          /* 2^11 * 3 * 5 */
    unsigned long long D = D0;
    seq_t prod = 1;

    /* compute factors */
    unsigned long long n64 = (unsigned long long)n;
    int sign = ((n & 1ULL) ? -1 : 1);            /* (-1)^n */

    /* factor 1 : 2*n+1-(-1)^n */
    unsigned long long f1 = 2ULL * n64 + 1ULL - (unsigned long long)sign;
    /* factor 2 : 2*n+5-(-1)^n */
    unsigned long long f2 = 2ULL * n64 + 5ULL - (unsigned long long)sign;
    /* factor 3 : 2*n+9-(-1)^n */
    unsigned long long f3 = 2ULL * n64 + 9ULL - (unsigned long long)sign;
    /* factor 4 : 2*n^2+15*n+51+3*(n-5)*(-1)^n */
    unsigned long long f4 = 2ULL * n64 * n64 + 15ULL * n64 + 51ULL
                           + 3ULL * (n64 - 5ULL) * (unsigned long long)sign;

    /* array of factors for easy looping */
    unsigned long long fac[4] = {f1, f2, f3, f4};

    for (int i = 0; i < 4; ++i) {
        unsigned long long f = fac[i];
        unsigned long long g = gcd_ull(f, D);
        D /= g;                         /* cancel gcd part from the divisor */
        unsigned long long f_red = f / g;

        /* multiply using 128‑bit intermediate to avoid overflow before
           the next cancellation */
        unsigned __int128 tmp = (unsigned __int128)prod * (unsigned __int128)f_red;
        prod = (seq_t)tmp;
    }

    /* D should now be 1, but just in case */
    if (D > 1) {
        prod /= (seq_t)D;
    }

    return prod;
}
