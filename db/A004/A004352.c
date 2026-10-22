/*
 * A004352  Binomial coefficient C(5n,n-10).
 * https://oeis.org/A004352
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(5n, n-10), computed multiplicatively as C(m,i) =
 *            C(m,i-1)*(m-i+1)/i
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 16, u64 n <= 25, u128 n <= 28,
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
    idx_t k = n - 10;
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
