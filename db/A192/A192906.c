/*
 * A192906  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-3) + a(n-4) initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 7;

    seq_t a0 = 1,  /* a(0) */
          a1 = 1,  /* a(1) */
          a2 = 2,  /* a(2) */
          a3 = 7;  /* a(3) */
    seq_t an;

    /* iterate from i = 4 up to n, keeping only the last four values */
    for (idx_t i = 4; i <= n; ++i) {
        an = 3 * a3 + a1 + a0;   /* a(i) = 3*a(i-1) + a(i-3) + a(i-4) */

        /* shift the sliding window: a0<-a1, a1<-a2, a2<-a3, a3<-an */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;   /* a3 holds a(n) after the loop */
}
