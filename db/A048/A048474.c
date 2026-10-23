/*
 * A048474  a(n) = 3*n*2^(n-1) + 1.
 * https://oeis.org/A048474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*2^(n-1) + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t pow = pow2(n - 1);
    return (((seq_t)n * 3) * pow) + (seq_t)1;
}
