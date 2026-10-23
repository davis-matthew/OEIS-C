/*
 * A016214  Expansion of g.f. 1/((1-x)*(1-3*x)(1-8*x)).
 * https://oeis.org/A016214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 24*a(n-2) + 1 (a(0)=1, a(1)=12)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a0 = (seq_t)1;         /* a(0) */
    seq_t a1 = (seq_t)12;        /* a(1) */
    seq_t an = a0;               /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        an = 11 * a1 - 24 * a0 + 1;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
