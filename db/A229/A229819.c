/*
 * A229819  Even bisection gives sequence a itself, n->a(2*(4*n+k)-1) gives
 *          k-th differences of a for k=1..4 with a(n)=n for n<2.
 * https://oeis.org/A229819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(0)=0, a(1)=1; for n≥2: * if n even: a(n)=a(n/2) * if n=8q+1:
 *            a(n)=a(q+1)-a(q) * if n=8q+3: a(n)=a(q+2)-2*a(q+1)+a(q) * if
 *            n=8q+5: a(n)=a(q+3)-3*a(q+2)+3*a(q+1)-a(q) * if n=8q+7:
 *            a(n)=a(q+4)-4*a(q+3)+6*a(q+2)-4*a(q+1)+a(q)
 * Range:     i8 n <= 302, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)n;
    if ((n & 1) == 0) {
        return a(n >> 1);
    }
    idx_t q = n >> 3; /* n / 8 */
    idx_t r = n & 7;   /* n % 8 */
    if (r == 1) {
        return a(q + 1) - a(q);
    } else if (r == 3) {
        seq_t t0 = a(q);
        seq_t t1 = a(q + 1);
        seq_t t2 = a(q + 2);
        return t2 - t1 * 2 + t0;
    } else if (r == 5) {
        seq_t t0 = a(q);
        seq_t t1 = a(q + 1);
        seq_t t2 = a(q + 2);
        seq_t t3 = a(q + 3);
        return t3 - t2 * 3 + t1 * 3 - t0;
    } else if (r == 7) {
        seq_t t0 = a(q);
        seq_t t1 = a(q + 1);
        seq_t t2 = a(q + 2);
        seq_t t3 = a(q + 3);
        seq_t t4 = a(q + 4);
        return t4 - t3 * 4 + t2 * 6 - t1 * 4 + t0;
    }
    return 0; /* unreachable */
}
