/*
 * A240684  Number of forests with n labeled nodes and 7 trees.
 * https://oeis.org/A240684
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-14) * (n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(n-1)*(n^6 +
 *            51*n^5 + 1385*n^4 + 24885*n^3 + 303766*n^2 + 2333976*n +
 *            8648640)/46080
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 19, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* For n < 7 the sequence is undefined; return 0 in that case. */
    if (n < 7) return 0;

    seq_t sn   = (seq_t)n;       /* type‑safe copy of n */
    seq_t denom = 46080;         /* denominator to be cancelled progressively */
    seq_t result = 1;            /* running product of the numerator */

    /* multiply (n‑6)…(n‑1) with cancellation against denom */
    for (idx_t k = n - 6; k <= n - 1; ++k) {
        seq_t f = (seq_t)k;
        seq_t g = gcd(f, denom);
        f   /= g;
        denom /= g;
        result *= f;
    }

    /* compute the 6‑th degree polynomial by Horner’s rule */
    seq_t poly = 1;
    poly = poly * sn + 51;
    poly = poly * sn + 1385;
    poly = poly * sn + 24885;
    poly = poly * sn + 303766;
    poly = poly * sn + 2333976;
    poly = poly * sn + 8648640;

    seq_t g = gcd(poly, denom);
    poly   /= g;
    denom  /= g;
    result *= poly;

    /* handle the n^(n‑14) term */
    if (n >= 14) {
        idx_t exp = n - 14;
        for (idx_t i = 0; i < exp; ++i) {
            seq_t f = sn;
            seq_t g2 = gcd(f, denom);
            f   /= g2;
            denom /= g2;
            result *= f;
        }
    } else {
        /* n < 14: factor 1 / n^(14‑n) by multiplying denom */
        idx_t exp = 14 - n;
        for (idx_t i = 0; i < exp; ++i) {
            denom *= sn;
        }
    }

    /* final division – the result is guaranteed to be integer */
    result /= denom;
    return result;
}
