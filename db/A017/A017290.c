/*
 * A017290  a(n) = (10*n + 1)^10.
 * https://oeis.org/A017290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 8, u128 n <= 713,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)10 + (seq_t)1;
    return pow_seq(base, 10);
}
