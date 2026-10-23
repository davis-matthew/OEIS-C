/*
 * A037793  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,1,0.
 * https://oeis.org/A037793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) + d(n-1) with d(n) = {3, 2, 1, 0} repeating;
 *            a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t digits[4] = {3, 2, 1, 0};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
        res = res * 4 + digits[i & 3];
    return res;
}
