/*
 * A037515  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1.
 * https://oeis.org/A037515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) + d(n), d(n)=2 if n mod 3 = 1, d(n)=0 if n mod 3
 *            = 2, d(n)=1 if n mod 3 = 0; a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* below the offset – behaviour undefined */

    seq_t val = 2;          /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t mod = i % 3;
        seq_t d = 0;
        if (mod == 1)
            d = 2;
        else if (mod == 2)
            d = 0;
        else
            d = 1;          /* mod == 0 */

        val = val * 6 + d;
    }

    return val;
}
