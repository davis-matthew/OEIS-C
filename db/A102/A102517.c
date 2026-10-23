/*
 * A102517  Expansion of (1+x^2)/((1-x+x^2)*(1+2*x^2)).
 * https://oeis.org/A102517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-4); Initial values:
 *            a(0)=1, a(1)=1, a(2)=-1, a(3)=-2
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)-2;

    /* iterative computation using a window of size 4 */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)-1;  /* a(n-2) */
    seq_t a3 = (seq_t)-2;  /* a(n-1) */
    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        next = a3 - (seq_t)3 * a2 + (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
