/*
 * A093659  First column of lower triangular matrix A093658; factorial of the
 *          number of 1's in binary expansion of n.
 * https://oeis.org/A093659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (number of 1’s in the binary representation of n)!
 * Range:     u8 n <= 62, u16 n <= 510, u32 n <= 8190, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is the factorial of the binary‐weight (popcount) of n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count 1’s in the binary representation of n. */
    idx_t pc = 0;
    idx_t x = n;
    while (x != 0) {
        pc += x & 1;
        x >>= 1;
    }

    /* Compute the factorial of the popcount. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= pc; ++i) {
        fact *= (seq_t)i;
    }

    return fact;
}
