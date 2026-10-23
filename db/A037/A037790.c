/*
 * A037790  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0,1.
 * https://oeis.org/A037790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number formed by the first n digits of the sequence
 *            3,2,0,1 in base 8 (pattern cycles every 4 digits)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int pattern[4] = {3, 2, 0, 1};
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        res = res * 8 + (seq_t)pattern[i & 3];
    }
    return res;
}
