/*
 * A036096  Centered cube numbers: (n+1)^18 + n^18.
 * https://oeis.org/A036096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^18 + n^18
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 10, u128 n <= 132,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

/* Computes the nth centered cube number:
 *     a(n) = (n+1)^18 + n^18
 * for n >= 0. */
seq_t a(idx_t n)
{
    seq_t term1 = pow_seq((seq_t)n + 1, 18);
    seq_t term2 = pow_seq((seq_t)n, 18);
    return term1 + term2;
}
