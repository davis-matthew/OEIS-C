/*
 * A085769  Total number of zeros in the decimal expansions of 2^n and 5^n.
 * https://oeis.org/A085769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   zeros_of_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the total number of zeros appearing in the decimal
   representations of 2ⁿ and 5ⁿ.
   The function computes the powers directly by repeated multiplication
   in base 10, using only integer arithmetic and dynamic arrays. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/zeros_of_power.h"

seq_t a(idx_t n)
{
    seq_t z2 = zeros_of_power(2, n);
    seq_t z5 = zeros_of_power(5, n);
    return z2 + z5;
}
