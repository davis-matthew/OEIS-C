/*
 * A004349  Binomial coefficient C(5n,n-7).
 * https://oeis.org/A004349
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, n-7)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_s(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t m = 5 * n;
    idx_t k = n - 7;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(m-k+i-1, i-1) -> C(m-k+i, i) = r * (m-k+i) / i, exactly */
        seq_t g = gcd_s(r, (seq_t)i);
        idx_t gi = (idx_t)(long long)g;
        idx_t d = i / gi;
        idx_t num = (m - k + i) / d;
        r /= g;
        r *= (seq_t)num;
    }
    return r;
}
