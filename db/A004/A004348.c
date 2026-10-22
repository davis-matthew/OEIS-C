/*
 * A004348  Binomial coefficient C(5n, n-6).
 * https://oeis.org/A004348
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, n-6)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 40,
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
    idx_t k = n - 6;
    idx_t m = 5 * n;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(m - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_s(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
