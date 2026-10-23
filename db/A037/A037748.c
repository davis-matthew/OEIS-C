/*
 * A037748  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,0,1.
 * https://oeis.org/A037748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base 8 digits are the first n terms of the periodic sequence 2,3,0,1. */
seq_t a(idx_t n)
{
    static const unsigned pat[4] = {2, 3, 0, 1};
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        val = val * (seq_t)8 + (seq_t)pat[i & 3];
    }
    return val;
}
