/*
 * A016965  a(n) = (6*n + 4)^9.
 * https://oeis.org/A016965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 22, u128 n <= 1000,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 6 + 4;
    return pow_seq(base, 9);
}
