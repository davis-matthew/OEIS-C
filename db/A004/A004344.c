/*
 * A004344  Binomial coefficient C(5n+10,n).
 * https://oeis.org/A004344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(5n+10,n) = Product_{i=1..n} (4n+10+i)/i, computed
 *            incrementally with gcd reduction so no intermediate exceeds the
 *            partial binomial C(4n+10+i,i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t num = (seq_t)n * 4 + 10 + (seq_t)i;
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq(r, den);
        r /= g;
        den /= g;
        num /= den;
        r *= num;
    }
    return r;
}
