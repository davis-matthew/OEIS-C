/*
 * A108208  a(n) = 4*(n-1)*a(n-1) -2*(n-1)*a(n-2), with a(0)=0, a(1)=2.
 * https://oeis.org/A108208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 2, a(n) = 4*(n-1)*a(n-1) - 2*(n-1)*a(n-2) for n
 *            >= 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    seq_t a0 = 0;   /* a(n-2) */
    seq_t a1 = 2;   /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t k   = (seq_t)(i - 1);           /* (n-1) as seq_t */
        seq_t term1 = a1 * k * 4;            /* 4*(n-1)*a(n-1) */
        seq_t term2 = a0 * k * 2;            /* 2*(n-1)*a(n-2) */
        seq_t an    = term1 - term2;         /* a(n) */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
