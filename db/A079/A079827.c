/*
 * A079827  a(n) = floor((n + reverse(n))/2).
 * https://oeis.org/A079827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor((n + reverse(n)) / 2)
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev_idx.h"

seq_t a(idx_t n)
{
    /* Compute the floor of (n + reverse(n)) / 2.
     * The cast to seq_t guarantees that the arithmetic is done in the
     * target integer type without overflow for the given n.
     */
    seq_t rev = rev_idx(n);
    seq_t sum = (seq_t)n + rev;
    return sum / 2;   /* integer division truncates toward 0 → floor */
}
