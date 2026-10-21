/*
 * A069685  Denominators of coefficients in -log(1+x)log(1-x) power series.
 * https://oeis.org/A069685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   Let H_m = Σ_{k=1..m} (-1)^{k+1}/k be the alternating harmonic
 *            number. The coefficient of x^{2n} in -log(1+x)log(1-x) is
 *            H_{2n-1}/n. a(n) is the denominator of this rational number in
 *            lowest terms. This routine computes that denominator
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the n-th term a(n) of OEIS A069685 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* has no defined meaning, all zero */

    /* Work with H_{2n-1} */
    idx_t m = 2 * n - 1;

    seq_t num = 0;          /* numerator of current H_k */
    seq_t den = 1;          /* denominator of current H_k */

    for (idx_t i = 1; i <= m; ++i)
    {
        /* term to add:   (-1)^{i+1} / i  */
        int sign = (i & 1) ? 1 : -1;

        seq_t new_num;
        if (sign == 1)
            new_num = num * i + den;   /* num/i + 1/i */
        else
            new_num = num * i - den;   /* num/i - 1/i */

        seq_t new_den = den * i;

        /* reduce fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* divide the harmonic number by n and reduce again */
    seq_t d_mul = den * n;                /* den * n */
    seq_t g2 = gcd(num, d_mul);
    seq_t denom = d_mul / g2;             /* final denominator */

    return denom;
}
