/*
 * A017265  a(n) = (9*n + 8)^9.
 * https://oeis.org/A017265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 14, u128 n <= 2122,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Compute (9*n + 8)^9 using the provided pow_seq helper */
    seq_t base = (seq_t)n * (seq_t)9 + (seq_t)8;
    return pow_seq(base, 9);
}
