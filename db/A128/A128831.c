/*
 * A128831  Number of n-tuples where each entry is chosen from the subsets of
 *          {1,2,3} such that the intersection of all n entries is empty.
 * https://oeis.org/A128831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 1)^3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* n is unsigned; a(1) should be 1. */
    seq_t two_pow = pow2(n);     /* 2^n in seq_t */
    seq_t m = two_pow - 1;     /* 2^n - 1 */
    return m * m * m;           /* (2^n - 1)^3 */
}
