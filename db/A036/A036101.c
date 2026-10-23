/*
 * A036101  Centered cube numbers: (n+1)^23 + n^23.
 * https://oeis.org/A036101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^23 + n^23
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 5, u128 n <= 45,
 *            big any n
 * Helpers:   pow23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow23.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t np1_seq = n_seq + (seq_t)1;
    seq_t p1 = pow23(np1_seq);
    seq_t p2 = pow23(n_seq);
    return p1 + p2;
}
