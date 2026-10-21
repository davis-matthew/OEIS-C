/*
 * A094943  A sequence generated from a semi-magic square.
 * https://oeis.org/A094943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+3) = 3*a(n+2) + 15*a(n+1) + 18*a(n), a(1) = 1, a(2) = 13,
 *            a(3) = 72
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1 */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)13;
    if (n == 3)
        return (seq_t)72;

    /* iterate using only the last three values */
    seq_t a1 = (seq_t)1;   /* a(k-2)   */
    seq_t a2 = (seq_t)13;  /* a(k-1)   */
    seq_t a3 = (seq_t)72;  /* a(k)     */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)3 * a3 + (seq_t)15 * a2 + (seq_t)18 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
