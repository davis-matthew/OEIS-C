/*
 * A127509  Number of n-tuples where each entry is chosen from the subsets of
 *          {1,2,3} such that the intersection of all n entries contains
 *          exactly one element.
 * https://oeis.org/A127509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2^n-1)^2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n);      /* 2^n */
    seq_t diff    = two_pow - 1;  /* 2^n - 1 */
    seq_t sq      = diff * diff;  /* (2^n - 1)^2 */
    return sq * 3;                /* 3*(2^n - 1)^2 */
}
