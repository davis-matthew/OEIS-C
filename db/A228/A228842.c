/*
 * A228842  Binomial transform of A014448.
 * https://oeis.org/A228842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 6, a(n) = 6 * a(n-1) - 4 * a(n-2) for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)2;
    if (n == (idx_t)1)
        return (seq_t)6;

    seq_t a0 = (seq_t)2;       /* a(0) */
    seq_t a1 = (seq_t)6;       /* a(1) */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t an = (seq_t)6 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
