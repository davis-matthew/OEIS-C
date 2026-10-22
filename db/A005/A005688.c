/*
 * A005688  Numbers of Twopins positions.
 * https://oeis.org/A005688
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) + 2*a(n-5) - 2*a(n-6) -
 *            a(n-10) for n >= 15, from the denominator
 *            (1-x^2-x^5)*(1-2*x+x^2-x^5) of the G.f.; a(5..14) = 1, 2, 3, 5,
 *            7, 10, 14, 20, 30, 45
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int a005688_init[10] = {1, 2, 3, 5, 7, 10, 14, 20, 30, 45};

seq_t a(idx_t n) {
    seq_t w[10];
    idx_t i;
    int j;
    if (n < 15) return (seq_t)a005688_init[n - 5];
    for (j = 0; j < 10; j++) w[j] = (seq_t)a005688_init[j];
    /* w[j] holds a(i-10+j) */
    for (i = 15; i <= n; i++) {
        seq_t v = w[9] * 2;
        v += w[6];
        v += w[5] * 2;
        v -= w[7] * 2;
        v -= w[4] * 2;
        v -= w[0];
        for (j = 0; j < 9; j++) w[j] = w[j + 1];
        w[9] = v;
    }
    return w[9];
}
