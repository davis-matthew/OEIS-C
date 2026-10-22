/*
 * A000858  Duplicate of A003436.
 * https://oeis.org/A000858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = P(n)/(2^(n-1)*(n-1)!), where P(n) = Integral_{x=0..oo}
 *            x^(n-1)*(x-2)^n*exp(-x) dx = Sum_{k=0..n}
 *            (-1)^k*binomial(n,k)*2^k*(2n-k-1)!. Integrating by parts gives
 *            the recurrence used below: a(n+1) = 2n*q(n) + q(n-1), q(n) =
 *            q(n-1) + a(n), with q(0) = 1, q(1) = 0. This gives a(2) = 1,
 *            a(3) = 4...
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t qprev, q, cur;
    idx_t i;
    if (n <= 1) return 0;
    qprev = 1;   /* q(0) */
    q = 0;       /* q(1) */
    cur = 0;
    for (i = 1; i + 1 <= n; i++) {
        cur = (seq_t)(2 * i) * q + qprev;   /* a(i+1) */
        if (i + 1 < n) {
            qprev = q;
            q = q + cur;                    /* q(i+1) */
        }
    }
    return cur;
}
