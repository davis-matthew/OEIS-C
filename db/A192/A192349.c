/*
 * A192349  Coefficient of x in the reduction (by x^2->x+1) of polynomial
 *          p(n,x) identified in Comments.
 * https://oeis.org/A192349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+8*a(n-2)-4*a(n-3)-4*a(n-4) Initial values:
 *            a(1)=0, a(2)=1, a(3)=2, a(4)=14
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset trivially */
    if (n == 0)
        return (seq_t)0;

    /* Base values for the first four terms */
    seq_t t0 = (seq_t)0;   /* a(1) */
    seq_t t1 = (seq_t)1;   /* a(2) */
    seq_t t2 = (seq_t)2;   /* a(3) */
    seq_t t3 = (seq_t)14;  /* a(4) */

    if (n <= 4)
        return (n == 1) ? t0 : (n == 2) ? t1 : (n == 3) ? t2 : t3;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = (seq_t)(2 * t3) + (seq_t)(8 * t2) - (seq_t)(4 * t1) - (seq_t)(4 * t0);
        /* Shift the window */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = val;
    }
    return t3;
}
