/*
 * A004325  Binomial coefficient C(3n,n-7).
 * https://oeis.org/A004325
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-7), computed multiplicatively as C(m,k) =
 *            prod_{i=1..k} (m-k+i)/i with gcd cancellation so no intermediate
 *            exceeds the final result
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 27, u128 n <= 50,
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
    idx_t m = 3 * n;
    idx_t k = n - 7;
    idx_t i;
    seq_t r = 1;
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
