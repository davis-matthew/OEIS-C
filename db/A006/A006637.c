/*
 * A006637  Expansion of (2 - x)^4/(1 - x)^8.
 * https://oeis.org/A006637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..4} binomial(4, k)*binomial(n+k+3, k+3)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 72, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned long long small_gcd(unsigned long long x, unsigned long long y) {
    while (y) {
        unsigned long long t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* binomial(m, k) computed multiplicatively, cancelling before multiplying */
static seq_t binom_seq(idx_t m, idx_t k) {
    seq_t c = 1;
    idx_t i;
    if (k > m) return 0;
    for (i = 0; i < k; i++) {
        unsigned long long d = (unsigned long long)(i + 1);
        unsigned long long r = (unsigned long long)(c % (seq_t)d);
        unsigned long long g = small_gcd(d, r);
        unsigned long long t;
        c /= (seq_t)g;
        t = d / g;
        c *= ((seq_t)(m - i)) / (seq_t)t;
    }
    return c;
}

seq_t a(idx_t n) {
    static const unsigned b4[5] = {1, 4, 6, 4, 1};
    seq_t result = 0;
    idx_t k;
    for (k = 0; k <= 4; k++) {
        result += (seq_t)b4[k] * binom_seq(n + k + 3, k + 3);
    }
    return result;
}
