/*
 * A004338  Binomial coefficient C(4n,n-8).
 * https://oeis.org/A004338
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(4n, n-8) = Product_{i=1..n-8} (3n+8+i)/i
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 15, u64 n <= 25, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_s(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t k = n - 8;
    idx_t base = 3 * n + 8;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t num = (seq_t)(base + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_s(r, den);
        r /= g;
        den /= g;
        r *= num / den;
    }
    return r;
}
