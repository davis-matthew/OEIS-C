/*
 * A034264  a(n) = f(n,4) where f is given in A034261.
 * https://oeis.org/A034264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n+1)*(n+4)*(n+3)*(n+2)*(n+1)/720
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Cancellation of the divisor 720 is performed by dividing its
   prime factors out of the multiplicands, avoiding overflow on
   64‑bit builds.  Works for all idx_t values. */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t factor[6];

    factor[0] = nn;                     /* n          */
    factor[1] = ((seq_t)5 * nn) + 1;    /* 5*n + 1    */
    factor[2] = nn + 4;                 /* n + 4      */
    factor[3] = nn + 3;                 /* n + 3      */
    factor[4] = nn + 2;                 /* n + 2      */
    factor[5] = nn + 1;                 /* n + 1      */

    seq_t div = 720;                    /* 2^4 * 3^2 * 5 */

    /* Pull all factors of 720 out of the multiplicands. */
    for (int i = 0; i < 6 && div > 1; ++i) {
        seq_t g = gcd(factor[i], div);
        if (g > 1) {
            div /= g;
            factor[i] /= g;
        }
    }

    /* The divisor must now be 1 (products contain all its primes). */
    seq_t res = 1;
    for (int i = 0; i < 6; ++i)
        res *= factor[i];

    return res;
}
