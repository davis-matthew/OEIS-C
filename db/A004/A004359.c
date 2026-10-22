/*
 * A004359  Binomial coefficient C(6n,n-4).
 * https://oeis.org/A004359
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6n, n-4) = Product_{i=1..n-4} (5n+4+i)/i
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 36,
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
    seq_t n_seq = (seq_t)n;
    seq_t result = 1;
    idx_t k = n - 4;

    for (idx_t i = 1; i <= k; i++) {
        seq_t num = 5 * n_seq + 4 + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = gcd_s(result, den);
        result /= g;
        den /= g;
        /* den now divides num since result*num/i is an integer */
        result *= num / den;
    }

    return result;
}
