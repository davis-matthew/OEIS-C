/*
 * A070834  a(n) = reverse(n)^n.
 * https://oeis.org/A070834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(n)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   _pow_seq_t, _reverse_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"
#include "../utils/_reverse_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
    {
        /* The sequence is defined for n ≥ 1; for n = 0 any value is acceptable. */
        return (seq_t)1;
    }

    seq_t rev = _reverse_idx(n);
    return _pow_seq_t(rev, n);
}
