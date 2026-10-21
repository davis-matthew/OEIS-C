/*
 * A099431  Expansion of x(1-2x+3x^2)/(1-x-2x)^2;.
 * https://oeis.org/A099431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=0, a(3)=6,
 *            a(n)=2*a(n-1)+3*a(n-2)-4*a(n-3)-4*a(n-4) for n>=4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)6;

    /* Sliding window of the last four terms: a[n-4], a[n-3], a[n-2], a[n-1] */
    seq_t a0 = (seq_t)0; /* a[n-4] when loop starts at i=4 */
    seq_t a1 = (seq_t)1; /* a[n-3] */
    seq_t a2 = (seq_t)0; /* a[n-2] */
    seq_t a3 = (seq_t)6; /* a[n-1] */

    seq_t an; /* a[i] */
    for (idx_t i = 4; i <= n; ++i)
    {
        an = (seq_t)2 * a3 + (seq_t)3 * a2 - (seq_t)4 * a1 - (seq_t)4 * a0;
        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an; /* For n>=4 this contains a(n). */
}
