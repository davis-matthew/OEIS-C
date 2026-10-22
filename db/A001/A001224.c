/*
 * A001224  If F(n) is the n-th Fibonacci number, then a(2n) = (F(2n+1) +
 *          F(n+2))/2 and a(2n+1) = (F(2n+2) + F(n+1))/2.
 * https://oeis.org/A001224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-5) - a(n-6) for n >= 7,
 *            a(1)=1, a(2)=2, a(3)=2, a(4)=4, a(5)=5, a(6)=9 (from G.f.
 *            (1+x)*(1-x-x^3)/((1-x-x^2)*(1-x^2-x^4)))
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w[6];
    idx_t i;
    int k;
    w[0] = (seq_t)1; w[1] = (seq_t)2; w[2] = (seq_t)2;
    w[3] = (seq_t)4; w[4] = (seq_t)5; w[5] = (seq_t)9;
    if (n <= 6) return w[n - 1];
    /* w[0..5] hold a(i-6)..a(i-1) */
    for (i = 7; i <= n; i++) {
        seq_t t = w[5] + w[4] + w[4];
        t -= w[3];
        t -= w[1];
        t -= w[0];
        for (k = 0; k < 5; k++) w[k] = w[k + 1];
        w[5] = t;
    }
    return w[5];
}
