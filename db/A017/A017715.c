/*
 * A017715  Binomial coefficients C(n,51).
 * https://oeis.org/A017715
 *
 * Offset:    51    (a(51) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,51) = (n choose 51)
 * Range:     u8 n <= 52, u16 n <= 54, u32 n <= 59, u64 n <= 74, u128 n <= 139,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 51 using multiplicative reduction to avoid
 * overflow as much as possible.  For any smaller n the function
 * returns 0 (the sequence is not defined there). */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Defined only for n >= 51 */
    if (n < 51)
        return 0;

    seq_t result = 1;

    /* Compute the product ∏_{k=1}^{51} (n-51 + k) / k
     * using gcd reductions before multiplication. */
    for (idx_t k = 1; k <= 51; ++k) {
        seq_t num = (seq_t)(n - 51 + k);   /* numerator term          */
        seq_t den = (seq_t)k;              /* denominator term        */

        /* Cancel common factors between result and den. */
        seq_t g1 = gcd(result, den);
        result /= g1;
        den /= g1;

        /* Cancel remaining common factors between num and den. */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;     /* den should now be 1 */

        /* Multiply accumulator by the reduced numerator. */
        result *= num;
    }

    return result;
}
