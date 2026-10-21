/*
 * A081321  a(n) = (2/3)*(2*n+1)*(2*n-1)!*binomial(3*n,2*n).
 * https://oeis.org/A081321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)(3n-1)!/n! (offset: a(1)=6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by direct multiplication: a(n) = (2n+1) * ∏_{k=n+1}^{3n-1} k */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* All calls are guaranteed for n >= 1, but guard against 0 */
    if (n == 0) return 0;

    /* Start with the factor (2n+1) */
    seq_t result = (seq_t)(2 * n + 1);

    /* Multiply by the product of integers from n+1 up to 3n-1 */
    for (idx_t k = n + 1; k <= 3 * n - 1; ++k) {
        result *= (seq_t)k;
    }

    return result;
}
