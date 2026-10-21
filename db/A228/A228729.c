/*
 * A228729  Product of the positive squares less than or equal to n.
 * https://oeis.org/A228729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (floor_sqrt(n)!)^2
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the product of all perfect squares ≤ n.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For the OEIS offset this returns 1 for the first term. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute floor(sqrt(n)) without floating point. */
    idx_t m = 0;
    for (idx_t k = 1; k <= n / k; ++k) {
        m = k;
    }

    /* Compute m! using multiplication in seq_t. */
    seq_t fact = 1;
    for (idx_t k = 2; k <= m; ++k) {
        fact *= (seq_t)k;
    }

    /* Return (m!)^2.  The multiplication is performed in seq_t. */
    seq_t result = fact;
    result *= fact;
    return result;
}
