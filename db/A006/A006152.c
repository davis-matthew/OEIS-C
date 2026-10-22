/*
 * A006152  Exponential generating function x*exp(x/(1-x)).
 * https://oeis.org/A006152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)*a(n-1) - (n^2-5*n+5)*a(n-2) - (n-4)*(n-2)*a(n-3),
 *            a(1) = 1, a(2) = 2, a(3) = 9
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p3, p2, p1, cur;
    idx_t i;
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)9;
    p3 = (seq_t)1;  /* a(i-3) */
    p2 = (seq_t)2;  /* a(i-2) */
    p1 = (seq_t)9;  /* a(i-1) */
    cur = p1;
    for (i = 4; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t c1 = (seq_t)2 * (seq_t)(i - 1);
        seq_t c2 = si * si - (seq_t)5 * si + (seq_t)5;
        seq_t c3 = (seq_t)(i - 4) * (seq_t)(i - 2);
        cur = c1 * p1;
        cur -= c2 * p2;
        cur -= c3 * p3;
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }
    return cur;
}
