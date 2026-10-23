/*
 * A022233  Gaussian binomial coefficients [ n,4 ] for q = 7.
 * https://oeis.org/A022233
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..4} (7^(n-i+1)-1)/(7^i-1)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;                     /* offset 4: values below are irrelevant */
    const seq_t q = (seq_t)7;                /* base 7 for the Gaussian binomial coefficient */

    seq_t num_prod = 1;                      /* running numerator of the rational product */
    seq_t den_prod = 1;                      /* running denominator of the rational product */

    for (int i = 1; i <= 4; ++i) {
        /* numerator = 7^(n-i+1) - 1 */
        seq_t num = pow_seq(q, (int)(n - i + 1));
        num -= 1;

        /* denominator = 7^i - 1 */
        seq_t den = pow_seq(q, i) - 1;

        /* cancel any common factor in this step */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply into the running product */
        num_prod *= num;
        den_prod *= den;

        /* further cancel any common factor between the two running products */
        seq_t g2 = gcd(num_prod, den_prod);
        num_prod /= g2;
        den_prod /= g2;
    }

    /* at this point the division is exact */
    return num_prod / den_prod;
}
