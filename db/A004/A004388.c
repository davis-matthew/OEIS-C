/*
 * A004388  Binomial coefficient C(8n,n-7).
 * https://oeis.org/A004388
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-7)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 13, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t g_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t m = 8 * n;
    idx_t k = n - 7;
    idx_t i;
    seq_t r = 1;
    if (k > m - k) k = m - k;
    /* r = C(m-k+i, i) at each step; reduce before multiplying to avoid overflow */
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = g_gcd(r, den);
        r /= g;
        den /= g;
        num /= den; /* exact: den is coprime to r and divides r*num */
        r *= num;
    }
    return r;
}
