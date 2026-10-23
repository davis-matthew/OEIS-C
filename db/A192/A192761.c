/*
 * A192761  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) Initial values:
 *            a(0)=0, a(1)=1, a(2)=5, a(3)=11
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    seq_t a0 = (seq_t)0;  /* a(n-4) */
    seq_t a1 = (seq_t)1;  /* a(n-3) */
    seq_t a2 = (seq_t)5;  /* a(n-2) */
    seq_t a3 = (seq_t)11; /* a(n-1) */
    seq_t a4;             /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        a4 = a3; a4 += a3; a4 += a3;   /* 3 * a(n-1) */
        a4 -= a2; a4 -= a2;             /* - 2 * a(n-2) */
        a4 -= a1;                       /* - a(n-3) */
        a4 += a0;                       /* + a(n-4) */

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
