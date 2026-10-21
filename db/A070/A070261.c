/*
 * A070261  4th diagonal of triangle defined in A051537.
 * https://oeis.org/A070261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n + 3, n) / gcd(n + 3, n)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = n*(n+3) / gcd(n, n+3)^2   (n >= 1) */

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* compute gcd(n, n+3) */
    idx_t m = n + 3u;
    idx_t g = gcd_idx(n, m);
    /* result = n*(n+3) / g^2 */
    seq_t numer = (seq_t)n * (seq_t)m;
    seq_t denom = (seq_t)g * (seq_t)g;
    return numer / denom;
}
