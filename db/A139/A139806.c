/*
 * A139806  a(n) = a(n-1)+a(n-2)+a(n-3)+2*a(n-4), a(0)=1, a(1)=3, a(2)=7,
 *          a(3)=15.
 * https://oeis.org/A139806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+2*a(n-4) with initial values a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=15 (OEIS A139806)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)15;

    /* Use a sliding window of the four most recent terms. */
    seq_t a0 = (seq_t)1;          /* a(n-4) */
    seq_t a1 = (seq_t)3;          /* a(n-3) */
    seq_t a2 = (seq_t)7;          /* a(n-2) */
    seq_t a3 = (seq_t)15;         /* a(n-1) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i)
    {
        ai = a3 + a2 + a1 + (seq_t)2 * a0; /* a(n) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;   /* a(n) */
}
