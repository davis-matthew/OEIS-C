/*
 * A202012  Expansion of (1-x+x^2)/((1-x)(1-x-x^2-x^3)).
 * https://oeis.org/A202012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-4) (n > 3) Initial values: a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=6
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;

    /* Use a sliding window of the last four values to apply the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-4)  */
    seq_t a1 = (seq_t)1;   /* a(n-3)  */
    seq_t a2 = (seq_t)3;   /* a(n-2)  */
    seq_t a3 = (seq_t)6;   /* a(n-1)  */
    seq_t an;              /* a(n)    */

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 + a3;            /* 2 * a(n-1) */
        an = an - a0;            /* - a(n-4)   */
        /* Shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;   /* a(n) */
}
