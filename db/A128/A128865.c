/*
 * A128865  Number of n-tuples where each entry is chosen from the subsets of
 *          {1,2,3,4} such that the intersection of all n entries contains
 *          exactly one element.
 * https://oeis.org/A128865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(2^n-1)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p = pow_uint(2, n);   /* compute 2^n */
    seq_t v = p - 1;            /* (2^n - 1) */
    seq_t res = v * v;          /* (2^n - 1)^2 */
    res = res * v;              /* (2^n - 1)^3 */
    res = res * 4;              /* 4*(2^n - 1)^3 */
    return res;
}
