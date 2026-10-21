/*
 * A234249  Number of ways to choose 4 points in an n X n X n triangular grid.
 * https://oeis.org/A234249
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(C(n + 1, 2), 4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 33, u64 n <= 538, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t N = (seq_t)n;
    seq_t m = (N + 1) * N / 2;            /* C(n+1,2) */

    seq_t res = 1;
    for (idx_t i = 1; i <= 4; ++i) {
        seq_t num = m - (i - 1);
        seq_t den = i;

        /* cancel factors between numerator and denominator */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* cancel remaining factor with the current result */
        seq_t g2 = gcd(res, den);
        res /= g2;
        den /= g2;   /* now den == 1 */

        res = res * num;
    }

    return res;
}
