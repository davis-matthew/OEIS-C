/*
 * A000122  Expansion of Jacobi theta function theta_3(x) = Sum_{m =-oo..oo}
 *          x^(m^2) (number of integer solutions to k^2 = n).
 * https://oeis.org/A000122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = 2 if n is a square, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a122_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a122_isqrt.h"

seq_t a(idx_t n)
{
    uint64_t r;
    if (n == 0) return (seq_t)1;
    r = a122_isqrt((uint64_t)n);
    if (r * r == (uint64_t)n) return (seq_t)2;
    return (seq_t)0;
}
