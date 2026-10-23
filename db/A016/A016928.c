/*
 * A016928  a(n) = (6*n + 1)^8.
 * https://oeis.org/A016928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 42, u128 n <= 2000,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 6 + 1;
    return pow_seq(base, 8);
}
