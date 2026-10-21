/*
 * A234659  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 17*a(n-2) - 492*a(n-3) + 1050*a(n-4) +
 *            1260*a(n-5) n ≥ 6, with initial values: a(1)=298, a(2)=1820,
 *            a(3)=11112, a(4)=68086, a(5)=417050
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first five terms */
    if (n == 1) return (seq_t)298;
    if (n == 2) return (seq_t)1820;
    if (n == 3) return (seq_t)11112;
    if (n == 4) return (seq_t)68086;
    if (n == 5) return (seq_t)417050;

    /* At this point n ≥ 6.  We keep only the last five values. */
    seq_t a1 = (seq_t)298;      /* a(n-5)   */
    seq_t a2 = (seq_t)1820;      /* a(n-4)   */
    seq_t a3 = (seq_t)11112;     /* a(n-3)   */
    seq_t a4 = (seq_t)68086;     /* a(n-2)   */
    seq_t a5 = (seq_t)417050;    /* a(n-1)   */

    seq_t cur = 0;
    idx_t  i;
    for (i = 6; i <= n; ++i)
    {
        /* Compute a(i): 11*a5 + 17*a4 + 1050*a2 + 1260*a1 - 492*a3 */
        cur = 11 * a5;
        cur += 17 * a4;
        cur += 1050 * a2;
        cur += 1260 * a1;
        cur -= 492 * a3;                /* positive minus smaller positive */

        /* Shift the window: a1<-a2, a2<-a3, a3<-a4, a4<-a5, a5<-cur */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = cur;
    }
    return cur;
}
