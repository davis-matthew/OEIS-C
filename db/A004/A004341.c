/*
 * A004341  Binomial coefficient C(4n,n-11).
 * https://oeis.org/A004341
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n, n-11), computed multiplicatively with gcd
 *            reduction
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 18, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned long long small_gcd(unsigned long long a, unsigned long long b) {
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t m = 4 * n;
    idx_t k = n - 11;
    idx_t i;
    seq_t r = 1;
    for (i = 1; i <= k; i++) {
        unsigned long long num = (unsigned long long)(m - k + i);
        unsigned long long den = (unsigned long long)i;
        unsigned long long rem = (unsigned long long)(r % (seq_t)den);
        unsigned long long g = small_gcd(den, rem);
        unsigned long long d2;
        r /= (seq_t)g;
        d2 = den / g;
        /* r*num is divisible by den and gcd(r, d2) = 1, so d2 divides num */
        r *= (seq_t)(num / d2);
    }
    return r;
}
