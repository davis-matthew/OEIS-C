/*
 * A130550  Denominators of partial sums for a series for 2*Zeta(2)/3 =
 *          (Pi^2)/9.
 * https://oeis.org/A130550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator( 2*∑_{i=1..n} 1/(i^2*binomial(2*i,i)) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum_num = 0;          /* numerator of the running sum   */
    seq_t sum_den = 1;          /* denominator of the running sum */
    const seq_t two = 2;
    seq_t binom = 2;            /* binomial(2,1), value of C(2*n,n) for n=1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        seq_t i2    = i_seq * i_seq;              /* i^2                 */
        seq_t d_i   = i2 * binom;                 /* denominator of 2/(i^2*C(2i,i)) */

        /* add 2/d_i to the current fraction sum_num/sum_den      */
        seq_t g1  = gcd(sum_den, d_i);
        seq_t lcm = (sum_den / g1) * d_i;          /* lcm of denominators */
        seq_t f1  = d_i / g1;                      /* d_i / g1            */
        seq_t f2  = sum_den / g1;                  /* sum_den / g1        */

        seq_t num1 = sum_num * f1;
        seq_t num2 = two * f2;
        seq_t new_num = num1 + num2;

        seq_t g2 = gcd(new_num, lcm);
        sum_num = new_num / g2;
        sum_den = lcm / g2;

        /* prepare binomial for next i (not needed after last step) */
        if (i < n)
        {
            seq_t mult = (seq_t)(4 * i + 2);        /* 4*i+2 = 2*(2*i+1)   */
            binom = (binom * mult) / (i + 1);      /* C(2*(i+1),i+1)      */
        }
    }

    return sum_den;
}
