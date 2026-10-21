/*
 * A071534  Determinant of n X n matrix of form : [1 2 1 0 0 0 0 0 0 0 / 2 1 2
 *          1 0 0 0 0 0 0 / 1 2 1 2 1 0 0 0 0 0 / 0 1 2 1 2 1 0 0 0 0 / 0 0 1
 *          2 1 2 1 0 0 0 / 0 0 0 1 2 1 2 1 0 0 / 0 0 0 0 1 2 1 2 1 0 / 0 0 0
 *          0 0 1 2 1 2 1 / 0 0 0 0 0 0 1 2 1 2 / 0 0 0 0 0 0 0 1 2 1].
 * https://oeis.org/A071534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -3*a(n-2) + 3*a(n-3) + a(n-5) for n ≥ 6 Initial values:
 *            a(1)= 1, a(2)= -3, a(3)= 0, a(4)=12, a(5)= -8 This recurrence
 *            comes from the generating function G.f.: -x*(1-3*x+x^4+3*x^2) /
 *            ((x-1)*(x^4+x^3+4*x^2+x+1))
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 35, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return  (seq_t)1;
    if (n == 2) return (seq_t)-3;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)12;
    if (n == 5) return (seq_t)-8;

    /* ring buffer holding the last 5 terms: a(i-5) .. a(i-1) */
    seq_t buf[5] = { (seq_t)1, (seq_t)-3, (seq_t)0, (seq_t)12, (seq_t)-8 };
    seq_t next;

    for (idx_t k = 6; k <= n; ++k) {
        /* a(k) = -3*a(k-2) + 3*a(k-3) + a(k-5) */
        next = - (seq_t)3 * buf[3] + (seq_t)3 * buf[2] + buf[0];

        /* shift buffer left by one: remove oldest term (buf[0]) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }
    return buf[4];  /* a(n) is now stored in buf[4] */
}
