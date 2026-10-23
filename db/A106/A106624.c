/*
 * A106624  Expansion of g.f.: (1 - x^2 + x^3)/((1-x^2)*(1-2*x^2)).
 * https://oeis.org/A106624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor(n/2) + floor((-1)^n - 1)/2
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    seq_t power = (seq_t)1;       /* compute 2^half by repeated left shift */
    for (idx_t i = 0; i < half; ++i)
        power <<= 1;
    return (n & 1) ? (power - 1) : power;
}
