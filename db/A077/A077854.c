/*
 * A077854  Expansion of 1/((1-x)*(1-2*x)*(1+x^2)).
 * https://oeis.org/A077854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 2*a(n-4); Initial
 *            values: a(0) = 1, a(1) = 3, a(2) = 6, a(3) = 12
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;

    /* Use a sliding window of the last four values. */
    seq_t a0 = (seq_t)1;      /* a(n-4) for n == 4 */
    seq_t a1 = (seq_t)3;      /* a(n-3) for n == 4 */
    seq_t a2 = (seq_t)6;      /* a(n-2) for n == 4 */
    seq_t a3 = (seq_t)12;     /* a(n-1) for n == 4 */
    seq_t an = a3;            /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        an = 3*a3 - 3*a2 + 3*a1 - 2*a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
