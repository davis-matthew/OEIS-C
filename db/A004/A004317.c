/*
 * A004317  Binomial coefficient C(2n,n-11).
 * https://oeis.org/A004317
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-11) = Product_{i=1..n-11} (n+11+i)/i, computed
 *            exactly with gcd reduction
 * Range:     u8 n <= 12, u16 n <= 15, u32 n <= 21, u64 n <= 36, u128 n <= 67,
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
    idx_t k = n - 11;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        seq_t x = (seq_t)(n + 11 + i);
        seq_t d = (seq_t)i;
        seq_t g = gcd_s(r, d);
        r /= g;
        d /= g;
        r *= (x / d);
    }
    return r;
}
