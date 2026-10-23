/*
 * A192747  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) with a(1)=0,
 *            a(2)=1, a(3)=6, a(4)=15
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)15;

    seq_t a1 = (seq_t)0;   /* a(n-4) */
    seq_t a2 = (seq_t)1;   /* a(n-3) */
    seq_t a3 = (seq_t)6;   /* a(n-2) */
    seq_t a4 = (seq_t)15;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = a4 * (seq_t)3;          /* 3*a(n-1) */
        cur -= a3 * (seq_t)2;         /* -2*a(n-2) */
        cur -= a2;                    /* -a(n-3) */
        cur += a1;                    /* +a(n-4) */
        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return a4;
}
