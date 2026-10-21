/*
 * A083448  Concatenation of first n numbers - n!. a(n) = 1234567891011... up
 *          to n - 1*2*3*4*5*6*7*8*9*10*11... up to n.
 * https://oeis.org/A083448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007908(n) - A000142(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t concat = 0;
    seq_t fact   = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* number of decimal digits of i */
        idx_t  tmp   = i;
        int    digs  = 0;
        while (tmp > 0)
        {
            ++digs;
            tmp /= 10;
        }

        /* multiplier 10^digs */
        seq_t  mult = 1;
        for (int d = 0; d < digs; ++d)
            mult *= 10;

        concat = concat * mult + (seq_t)i;
        fact   = fact * (seq_t)i;
    }

    return concat - fact;
}
