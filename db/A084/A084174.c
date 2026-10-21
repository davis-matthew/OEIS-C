/*
 * A084174  a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 2*a(n-4).
 * https://oeis.org/A084174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=6,
 *            a(n)=3*a(n-1)-a(n-2)-3*a(n-3)+2*a(n-4) for n>=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;

    /* Use a sliding window of the last four values. */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)3;   /* a(n-2) */
    seq_t a3 = (seq_t)6;   /* a(n-1) */
    seq_t a4;              /* a(n) */

    for (idx_t i = 4; i <= n; ++i)
    {
        a4 = (seq_t)3 * a3 - a2 - (seq_t)3 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a4;
}
