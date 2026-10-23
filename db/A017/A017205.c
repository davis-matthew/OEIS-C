/*
 * A017205  a(n) = (9*n + 3)^9.
 * https://oeis.org/A017205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 3)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 15, u128 n <= 2123,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * ((seq_t)9);
    base += ((seq_t)3);
    return pow_seq(base, 9);
}
