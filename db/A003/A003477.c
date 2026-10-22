/*
 * A003477  Expansion of 1/((1-2x)(1+x^2)(1-x-2x^3)).
 * https://oeis.org/A003477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 5*a(n-3) - 6*a(n-4) + 2*a(n-5) -
 *            4*a(n-6), a(0) = 1, a(n) = 0 for -5 <= n <= -1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* w[k] holds a(i-1-k) */
    seq_t w[6];
    int k;
    idx_t i;
    for (k = 0; k < 6; k++) w[k] = 0;
    seq_t cur = 1; /* a(0) */
    for (i = 1; i <= n; i++) {
        for (k = 5; k > 0; k--) w[k] = w[k - 1];
        w[0] = cur;
        seq_t pos = (seq_t)3 * w[0] + (seq_t)5 * w[2] + (seq_t)2 * w[4];
        seq_t neg = (seq_t)3 * w[1] + (seq_t)6 * w[3] + (seq_t)4 * w[5];
        cur = pos - neg;
    }
    return cur;
}
