/*
 * A015255  Gaussian binomial coefficient [ n,2 ] for q = -5.
 * https://oeis.org/A015255
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) + 105*a(n-2) - 125*a(n-3), with a(2)=1,
 *            a(3)=21, a(4)=546. The sequence is computed by iterating the
 *            recurrence. For n<2 the value is undefined
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined below offset */

    /* trivial cases */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)21;
    if (n == 4) return (seq_t)546;

    /* initialise the three previous values: a(2), a(3), a(4) */
    seq_t a2 = (seq_t)1;    /* a(2) */
    seq_t a3 = (seq_t)21;   /* a(3) */
    seq_t a4 = (seq_t)546;  /* a(4) */
    seq_t ai;

    /* iterate from n = 5 up to the requested n */
    for (idx_t i = (idx_t)5; i <= n; ++i)
    {
        ai = (seq_t)21 * a4 + (seq_t)105 * a3 - (seq_t)125 * a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;
}
