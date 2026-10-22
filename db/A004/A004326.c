/*
 * A004326  Binomial coefficient C(3n,n-8).
 * https://oeis.org/A004326
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n, n-8)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 17, u64 n <= 28, u128 n <= 51,
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
    idx_t k = n - 8;
    idx_t m = 3 * n;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_s(r, den);
        r /= g;
        den /= g;
        r *= num / den;
    }
    return r;
}
