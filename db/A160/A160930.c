/*
 * A160930  a(n) = n + reversal(n-1) + reversal(n+1).
 * https://oeis.org/A160930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + rev(n-1) + rev(n+1)
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"

seq_t a(idx_t n)
{
    idx_t n_minus = n - 1;          /* n ≥ 1 guarantees no underflow */
    seq_t res   = (seq_t)n;
    seq_t rev1  = (seq_t)rev(n_minus);
    seq_t rev2  = (seq_t)rev(n + 1);
    return res + rev1 + rev2;
}
