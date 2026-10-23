/*
 * A022206  Gaussian binomial coefficients [ n,7 ] for q = 4.
 * https://oeis.org/A022206
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{i=1}^{7} (4^(n-i+1)-1)/(4^i-1)
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   gcd, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow4.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 7; values for n < 7 are unspecified. */
    if (n < 7) return 0;

    seq_t res = 1;   /* current product */
    for (idx_t i = 1; i <= 7; ++i)
    {
        seq_t num = pow4(n - i + 1) - 1;  /* 4^(n-i+1) - 1 */
        seq_t den = pow4(i) - 1;          /* 4^i - 1 */

        /* cancel common factors with the current product */
        seq_t g1 = gcd(res, den);
        if (g1 != 1) {
            res /= g1;
            den  /= g1;
        }

        /* cancel remaining common factors between numerator and denom */
        seq_t g2 = gcd(num, den);
        if (g2 != 1) {
            num /= g2;
            den  /= g2;
        }

        /* after cancellations den should be 1; if not, divide after multiply */
        res *= num;
        if (den != 1) res /= den;
    }
    return res;
}
