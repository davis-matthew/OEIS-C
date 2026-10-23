/*
 * A037759  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1,2.
 * https://oeis.org/A037759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3, a(n) = a(n-1)*5 + d(n), where d(n) = {3,0,1,2}[(n-1)
 *            mod 4]
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned int pattern[4] = {3, 0, 1, 2};
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * 5 + pattern[i % 4];
    }
    return val;
}
