/*
 * A004377  Binomial coefficient C(7n,n-9).
 * https://oeis.org/A004377
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n, n-9) = Product_{i=0..n-10} (6n+10+i)/(i+1)
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 15, u64 n <= 22, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned long long small_gcd(unsigned long long x, unsigned long long y) {
    while (y) { unsigned long long t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n) {
    idx_t m = 7 * n;
    idx_t k = n - 9;
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < k; i++) {
        /* r = C(m, i); next r = C(m, i+1) = r * (m - i) / (i + 1) */
        unsigned long long d = (unsigned long long)(i + 1);
        unsigned long long rm = (unsigned long long)(r % (seq_t)d);
        unsigned long long g = small_gcd(d, rm);   /* gcd(r, d) */
        unsigned long long d2 = d / g;              /* divides (m - i) */
        unsigned long long f = (unsigned long long)(m - i) / d2;
        r = (r / (seq_t)g) * (seq_t)f;
    }
    return r;
}
