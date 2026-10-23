/*
 * A129953  First differences of A129952.
 * https://oeis.org/A129953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(1) = 1; a(n) = (n+2)*2^(n-2) for n >= 2
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    return (seq_t)(n + 2) * pow2((idx_t)(n - 2));
}
