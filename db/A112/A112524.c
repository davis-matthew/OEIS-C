/*
 * A112524  a(n) = a(n-1) + 2*n^2 with a(1) = 1.
 * https://oeis.org/A112524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(2n+1)/3 - 1
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure n >= 1; the function is only defined for n >= 1. */
    if (n == 0) return 0;  /* undefined, but protects against overflow */

    seq_t x = (seq_t)n;
    seq_t y = ((seq_t)n) + 1;
    seq_t z = 2 * (seq_t)n + 1;

    /* Divide one of the factors by 3 to keep the intermediate product small. */
    if (x % 3 == 0) {
        x /= 3;
    } else if (y % 3 == 0) {
        y /= 3;
    } else {
        z /= 3;
    }

    /* Compute the product and subtract 1. */
    seq_t result = x * y;
    result = result * z;
    result -= 1;
    return result;
}
