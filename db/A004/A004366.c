/*
 * A004366  Binomial coefficient C(6n,n-11).
 * https://oeis.org/A004366
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6n, n-11) = Product_{i=1..n-11} (5n+11+i)/i
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 17, u64 n <= 25, u128 n <= 28,
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
    idx_t k = n - 11;
    seq_t base = (seq_t)5 * (seq_t)n + 11;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; i++) {
        seq_t num = base + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = gcd_s(result, den);
        result /= g;
        den /= g;
        /* result*num is divisible by den and gcd(result, den) = 1, so den | num */
        result *= num / den;
    }
    return result;
}
