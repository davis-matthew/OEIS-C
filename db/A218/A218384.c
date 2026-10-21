/*
 * A218384  Number of nonempty subsets S of the powerset of a set of size n
 *          that have the even intersection property.
 * https://oeis.org/A218384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2*Sum_{i=0..n-1} (-1)^(n-i-1) * (2^(2^i-1)-1) * *
 *            (Π_{j=1..i} (2^(n-j+1)-1)/(2^j-1)) * 2^binomial(n-i,2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_seq.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset, but return 0 */

    seq_t result = (seq_t)1;          /* the leading 1 */
    seq_t prod   = (seq_t)1;          /* product Π_{j=1..i} ...  */

    for (idx_t i = 0; i < n; ++i)
    {
        /* sign of the term */
        int sign = (((n - i - 1) & 1) ? -1 : 1);

        /* 2^i and then 2^(2^i-1) */
        seq_t two_pow_i = pow_seq((seq_t)2, (int)i);    /* 2^i */
        seq_t exp       = two_pow_i - (seq_t)1;         /* 2^i - 1 */
        seq_t pow2_exp  = big_pow((seq_t)2, exp);      /* 2^(2^i-1) */
        seq_t part      = pow2_exp - (seq_t)1;          /* (2^(2^i-1)-1) */

        /* binary exponent binomial(n-i,2) */
        seq_t bin = binomial((idx_t)(n - i), (idx_t)2);
        seq_t pow2_bin = big_pow((seq_t)2, bin);       /* 2^bin */

        /* combine the factors */
        seq_t mult = part * prod;
        mult *= pow2_bin;

        /* add or subtract 2*mult to the result */
        if (sign == 1)
            result += (seq_t)2 * mult;
        else
            result -= (seq_t)2 * mult;

        /* update prod for next i (if any) */
        if (i < n - 1)
        {
            seq_t numerator   = pow_seq((seq_t)2, (int)(n - i)) - (seq_t)1;
            seq_t denominator = pow_seq((seq_t)2, (int)(i + 1)) - (seq_t)1;
            prod = (prod * numerator) / denominator;   /* exact division */
        }
    }

    return result;
}
