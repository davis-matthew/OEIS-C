/*
 * A183907  Number of nondecreasing arrangements of n+2 numbers in 0..4 with
 *          each number being the sum mod 5 of two others.
 * https://oeis.org/A183907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 18 n^3 + 119 n^2 – 570 n + 552) / 24 for n > 1 *
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 180, u64 n <= 180,
 *            u128 n <= 180, big any n
 * Helpers:   polynomial_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/polynomial_value.h"

seq_t a(idx_t n)
{
    return polynomial_value(n);
}
