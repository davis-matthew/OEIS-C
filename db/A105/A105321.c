/*
 * A105321  Convolution of binomial(1,n) and Gould's sequence A001316.
 * https://oeis.org/A105321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^{popcount(n)} + 2^{popcount(n-1)}) for n > 0, a(0) = 1
 *            where popcount(x) is the number of 1-bits in the binary
 *            expansion of x
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses only integer arithmetic and no libraries. */

#include <stdint.h>
#include "../utils/pow2_popcount.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1; /* a(0) = 1 */
    }
    seq_t v1 = pow2_popcount(n);
    seq_t v0 = pow2_popcount(n - 1);
    return v1 + v0;
}
