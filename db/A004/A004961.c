/*
 * A004961  a(n) = ceiling(n*phi^6), where phi is the golden ratio.
 * https://oeis.org/A004961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n + ceil(sqrt(80*n^2))
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t result = 9 * n_seq;
    if (n > 0) {
        uint64_t m = a000037_isqrt(80 * n * n);
        result += m + 1;
    }
    return result;
}
