/*
 * A010973  a(n) = binomial(n,20).
 * https://oeis.org/A010973
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,20)
 * Range:     u8 n <= 22, u16 n <= 25, u32 n <= 35, u64 n <= 86, u128 n <= 710,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 20; values below this index are irrelevant. */
    if (n < 20)
        return 0;

    seq_t result = 1;

    /* Compute the product (n-20+1)*(n-20+2)*...*(n-20+20) / 20!  */
    for (idx_t k = 1; k <= 20; ++k)
    {
        /* Numerator term, cast to seq_t */
        seq_t numerator = (seq_t)(n - 20 + k);

        /* Denominator term = k */
        seq_t denom = k;

        /* Cancel common factors between current result and denom */
        seq_t g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* Cancel any remaining common factors with the numerator */
        g = gcd(numerator, denom);
        numerator /= g;
        denom /= g;  /* denom should become 1 */

        /* Multiply the reduced numerator into the result */
        result = result * numerator;
    }

    return result;
}
