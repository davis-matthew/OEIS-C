/*
 * A026937  a(n) = Sum_{k=0..n} (k+1)*T(n, n-k), where T is given by A008288.
 * https://oeis.org/A026937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4) with initial
 *            values a(0)=1, a(1)=3, a(2)=10, a(3)=30
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)30;

    /* iterate using the recurrence, keeping only the last four values */
    seq_t a0 = (seq_t)1;    /* a(n-4) */
    seq_t a1 = (seq_t)3;    /* a(n-3) */
    seq_t a2 = (seq_t)10;   /* a(n-2) */
    seq_t a3 = (seq_t)30;   /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* sub = 2*a2 + 4*a1 + a0 */
        seq_t sub = 2 * a2 + 4 * a1 + a0;
        an = 4 * a3 - sub;

        /* shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
