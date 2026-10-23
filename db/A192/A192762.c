/*
 * A192762  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-2*a(n-2)-a(n-3)+a(n-4); Initial values: a(0)=0,
 *            a(1)=1, a(2)=6, a(3)=13
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)13;

    /* variables holding the last four values of the sequence */
    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)6;   /* a(n-2) */
    seq_t a3 = (seq_t)13;  /* a(n-1) */
    seq_t next;

    /* iterate from 4 up to n, computing one new value from the four
       most recent terms each time */
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)(3 * a3 - 2 * a2 - a1 + a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    /* a3 now holds a(n) */
    return a3;
}
