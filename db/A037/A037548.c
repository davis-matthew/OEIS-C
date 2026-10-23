/*
 * A037548  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,2.
 * https://oeis.org/A037548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*7 + d_n where d_n = (n % 3 == 1) ? 1 : 2 The
 *            sequence is the base‑7 integer formed by the first n digits of
 *            the repeating pattern 1,2,2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Offset 1: a(1) = 1 */
    if (n == 0)      /* not used in the sequence, but harmless */
        return 0;

    seq_t val = 1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        val = val * 7;
        val += (i % 3 == 1) ? 1 : 2;
    }

    return val;
}
