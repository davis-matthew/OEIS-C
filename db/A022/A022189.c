/*
 * A022189  Gaussian binomial coefficients [n, 6] for q = 2.
 * https://oeis.org/A022189
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..6} (2^(n-i+1)-1)/(2^i-1)
 * Range:     u8 none, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 6 (offset). Return 0 for smaller n. */
    if (n < 6)
        return (seq_t)0;

    /* Denominators 2^i-1 for i = 1..6 */
    static const seq_t denom[6] = { 1, 3, 7, 15, 31, 63 };

    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= 6; ++i)
    {
        idx_t exp = n + 1 - i;                   /* exponent for the numerator term */
        seq_t numerator = pow2(exp) - (seq_t)1;   /* 2^exp - 1                     */
        seq_t den = denom[i - 1];

        /* Reduce the fraction (numerator/den) before multiplying */
        seq_t g1 = gcd(numerator, den);
        numerator /= g1;
        den /= g1;

        /* Reduce any remaining factor of den with the current result */
        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;

        /* After these reductions den should be 1 (integer division). */
        result = result * numerator;
        if (den != 1)
            result /= den;
    }

    return result;
}
