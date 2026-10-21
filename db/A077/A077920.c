/*
 * A077920  Expansion of (1-x)^(-1)/(1+2*x-x^2-x^3).
 * https://oeis.org/A077920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 3*a(n-2) - a(n-4); Initial values: a(0) = 1,
 *            a(1) = -1, a(2) = 4, a(3) = -7
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)-7;

    /* t0..t3 hold a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t t0 = (seq_t)1;   /* a(0) */
    seq_t t1 = (seq_t)-1;  /* a(1) */
    seq_t t2 = (seq_t)4;   /* a(2) */
    seq_t t3 = (seq_t)-7;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = -(t3) + (seq_t)3 * t2 - t0;  /* a(i) = -a(i-1)+3*a(i-2)-a(i-4) */
        /* shift the window */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
