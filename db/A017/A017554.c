/*
 * A017554  a(n) = (12*n + 2)^10.
 * https://oeis.org/A017554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+2)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 594,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)2;
    return pow_seq(base, 10);
}
