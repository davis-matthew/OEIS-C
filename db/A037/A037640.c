/*
 * A037640  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1.
 * https://oeis.org/A037640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=2; for n>1, a(n)=5*a(n-1)+digit(n), where the digits repeat
 *            2,3,1 in that order
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t digit;
        idx_t r = i % 3;
        if (r == 1)
            digit = 2;
        else if (r == 2)
            digit = 3;
        else
            digit = 1;
        val = val * 5 + digit;
    }
    return val;
}
