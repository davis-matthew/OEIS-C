/*
 * A008817  Expansion of (1+x^10)/((1-x)^2*(1-x^10)).
 * https://oeis.org/A008817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-8)*floor((n+1)/10) - 10*floor((n+1)/10)^2 + n + 1
 * Range:     u8 n <= 49, u16 n <= 808, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor((n+1)/10) as an unsigned seq_t value. */
    seq_t q = (seq_t)((n + 1) / 10);

    /* Compute 2*n - 8 using a signed integer to handle negative values for n < 4. */
    int64_t t = 2 * (int64_t)n - 8;

    /* Calculate the three components of the formula in seq_t arithmetic. */
    seq_t part1 = (seq_t)t * q;
    seq_t part2 = (seq_t)10 * q * q;
    seq_t part3 = (seq_t)(n + 1);

    /* Combine the parts to obtain a(n). */
    return part1 - part2 + part3;
}
