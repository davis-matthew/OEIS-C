/*
 * A005126  a(n) = 2^n + n + 1.
 * https://oeis.org/A005126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_seq.h"

seq_t a(idx_t n) {
    return pow2_seq(n) + (seq_t)n + 1;
}
