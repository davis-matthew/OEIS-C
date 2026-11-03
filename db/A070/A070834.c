/*
 * A070834  a(n) = reverse(n)^n.
 * https://oeis.org/A070834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(n)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 98 (2026-11-01)
 * Helpers:   ipow_idx, reverse_digits_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"
#include "../utils/reverse_digits_idx_to_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
    {
        /* The sequence is defined for n ≥ 1; for n = 0 any value is acceptable. */
        return (seq_t)1;
    }

    seq_t rev = reverse_digits_idx_to_seq(n);
    return ipow_idx(rev, n);
}
