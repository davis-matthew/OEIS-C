/*
 * A081902  Third binomial transform of binomial(n+5, 5).
 * https://oeis.org/A081902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(n^5 + 90*n^4 + 2635*n^3 + 29850*n^2 + 121024*n +
 *            122880)/122880
 * Range:     u8 none, u16 n <= 0, u32 n <= 12, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   gcd, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    const seq_t DEN = 122880;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n2 * n2;
    seq_t n5  = n4 * N;

    seq_t poly = n5
                + (seq_t)90 * n4
                + (seq_t)2635 * n3
                + (seq_t)29850 * n2
                + (seq_t)121024 * N
                + (seq_t)122880;

    seq_t pow4_n = pow4((idx_t)n);

    seq_t den = DEN;
    seq_t g;

    /* cancel common factors between poly and denominator */
    g = gcd(poly, den);
    if (g > 1) {
        poly /= g;
        den /= g;
    }

    /* cancel common factors between pow4_n and remaining denominator */
    g = gcd(pow4_n, den);
    if (g > 1) {
        pow4_n /= g;
        den /= g;
    }

    /* multiply the reduced parts and divide any remaining denominator */
    seq_t res = pow4_n * poly;
    if (den != 1) {
        res /= den;
    }

    return res;
}
