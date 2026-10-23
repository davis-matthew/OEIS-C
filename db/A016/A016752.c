/*
 * A016752  a(n) = (2*n)^12.
 * https://oeis.org/A016752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^12
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 20, u128 n <= 812,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t base = (seq_t)n * (seq_t)2;
    return pow_seq(base, 12);
}
