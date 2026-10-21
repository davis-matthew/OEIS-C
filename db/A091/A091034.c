/*
 * A091034  Fourth column (k=5) of array A090438 ((4,2)-Stirling2) divided by
 *          24.
 * https://oeis.org/A091034
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2)*(2*n-3)*(2*n)!/(5!*(3!)^2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (n-1)*(n-2)*(2*n-3)*(2*n)!/4320           */
/* Recurrence: a(3)=1,  for n>=4,
 *   a(n) = a(n-1) * (n-1)*(2n-3)*(2n)*(2n-1) / ((n-3)*(2n-5))   */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) {
        /* Undefined for indices below the offset. */
        return 0;
    }
    if (n == 3) {
        return 1;
    }

    seq_t result = 1;                 /* a(3) */

    for (idx_t k = 4; k <= n; ++k) {
        /* Build the product (k-1)*(2k-3)*(2k)*(2k-1) */
        seq_t num = (seq_t)(k - 1);
        num *= (seq_t)(2 * k - 3);
        num *= (seq_t)(2 * k);
        num *= (seq_t)(2 * k - 1);

        /* Denominator (k-3)*(2k-5) */
        seq_t den = (seq_t)(k - 3);
        den *= (seq_t)(2 * k - 5);

        /* Cancel common factors with the current result */
        seq_t g = gcd(result, den);
        if (g > 1) {
            result /= g;
            den /= g;
        }

        /* Cancel remaining denominator factors with the numerator */
        g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* After cancellation den should be 1 */
        result *= num;
    }

    return result;
}
