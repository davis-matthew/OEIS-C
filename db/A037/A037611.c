/*
 * A037611  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2.
 * https://oeis.org/A037611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n>1, a(n)=4*a(n-1)+d_n where d_n cycles 1,3,2 (i.e.,
 *            d_n=1 if (n-1)%3==0, 3 if (n-1)%3==1, 2 otherwise)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    static const seq_t digits[3] = {1, 3, 2};
    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * 4 + digits[(i - 1) % 3];
    }
    return res;
}
