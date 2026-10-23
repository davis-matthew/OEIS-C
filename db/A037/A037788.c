/*
 * A037788  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0,1.
 * https://oeis.org/A037788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6 * a(n-1) + d_n, with d_n the n-th digit of the periodic
 *            sequence 3,2,0,1 (index start at 1, offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS A037788 value for index n. */
seq_t a(idx_t n)
{
    seq_t val = 0;
    const unsigned char pattern[4] = {3, 2, 0, 1};

    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * 6 + pattern[(i - 1) & 3];
    }

    return val;
}
