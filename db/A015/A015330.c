/*
 * A015330  Gaussian binomial coefficient [ n,6 ] for q = -7.
 * https://oeis.org/A015330
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,6] for q = -7, i.e.
 *            ∏_{i=0}^{5} (1 - (-7)^{\,n-i}) / (1 - (-7)^{\,i+1})
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* offset is 6 – values below are irrelevant */
    if (n < 6)
        return 0;

    seq_t res_num = (seq_t)1;
    seq_t res_den = (seq_t)1;

    for (idx_t i = 0; i < 6; ++i)
    {
        idx_t exp_num = n - i;          /* exponent for numerator factor   */
        idx_t exp_den = i + 1;          /* exponent for denominator factor */

        /* 7^exp, then apply 1 ± 7^exp according to parity */
        seq_t pow_num = big_pow((seq_t)7, (seq_t)exp_num);
        seq_t pow_den = big_pow((seq_t)7, (seq_t)exp_den);

        seq_t num_factor = (exp_num & 1) ? (pow_num + (seq_t)1)
                                         : (pow_num - (seq_t)1);
        seq_t den_factor = (exp_den & 1) ? (pow_den + (seq_t)1)
                                         : (pow_den - (seq_t)1);

        /* Reduce the current fraction by gcd(num_factor, den_factor) */
        seq_t g = seq_gcd(num_factor, den_factor);
        if (g != (seq_t)1)
        {
            num_factor /= g;
            den_factor /= g;
        }

        /* Cancel common factors with the accumulated result */
        g = seq_gcd(res_num, den_factor);
        if (g != (seq_t)1)
        {
            res_num /= g;
            den_factor /= g;
        }

        g = seq_gcd(res_den, num_factor);
        if (g != (seq_t)1)
        {
            res_den /= g;
            num_factor /= g;
        }

        /* Multiply the simplified factors into the result */
        res_num *= num_factor;
        res_den *= den_factor;
    }

    /* The result is guaranteed to be an integer: res_den should be 1 */
    return res_num / res_den;
}
