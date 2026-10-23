/*
 * A014050  a(n) = (n^2+1)^n.
 * https://oeis.org/A014050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2+1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)n + 1;
    return seq_pow(base, n);
}
