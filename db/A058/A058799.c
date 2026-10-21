/*
 * A058799  Column 2 of A007754.
 * https://oeis.org/A058799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*a(n-1)-a(n-2) with a(0)=1, a(-1)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* a(0) and a(1) */
    seq_t a0 = (seq_t)1;   /* a(n-2) when starting the loop */
    seq_t a1 = (seq_t)3;   /* a(n-1) when starting the loop */

    /* Iterate from n=2 up to the requested n */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t ai = ((seq_t)(i + 2)) * a1 - a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
