/*
 * A037612  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2.
 * https://oeis.org/A037612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the base‑5 number whose digits are the first n terms of
 *            the periodic sequence 1,3,2,1,3,2,… (recurrence a(1)=1,
 *            a(n)=5*a(n-1)+digit(n) where digit(n) = {1,3,2})
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t d;
        if ((i % 3) == 0)
            d = 1;
        else if ((i % 3) == 1)
            d = 3;
        else
            d = 2;
        val = val * 5 + d;
    }
    return val;
}
