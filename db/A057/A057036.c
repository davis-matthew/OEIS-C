/*
 * A057036  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057036(n)=i(2n-1).
 * https://oeis.org/A057036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i(2n-1), where the integer k is placed in the rectangle
 *            R(i,j) with R(i,j) = C(i+j-1,2)+i. This implementation computes
 *            the i‑coordinate of the integer in the odd position (2n–1)
 * Range:     u8 n <= 68, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   odd_position_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/odd_position_i.h"

seq_t a(idx_t n)
{
    return odd_position_i(n);
}
