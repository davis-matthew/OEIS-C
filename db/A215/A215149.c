/*
 * A215149  a(n) = n * (1 + 2^(n-1)).
 * https://oeis.org/A215149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (1 + 2^(n-1))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t pow = pow2(n - 1);          /* 2^(n-1) */
    seq_t sum = pow + (seq_t)1;       /* 1 + 2^(n-1) */
    return (seq_t)n * sum;           /* n * (1 + 2^(n-1)) */
}
