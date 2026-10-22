/*
 * A004347  Binomial coefficient C(5n,n-5).
 * https://oeis.org/A004347
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n, n-5), computed multiplicatively: C(m,0) =
 *            1, C(m,i) = C(m,i-1) * (m-k+i) / i with m = 5n, k = n-5,
 *            dividing out gcd first so intermediates never exceed the final
 *            value
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t g_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t m = 5 * n;
    idx_t k = n - 5;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        uint64_t num = (uint64_t)(m - k + i);
        uint64_t den = (uint64_t)i;
        uint64_t rm = (uint64_t)(r % (seq_t)den);
        uint64_t g = g_gcd(den, rm);
        /* r*num/i is an integer; with g = gcd(r,i), (i/g) divides num */
        r /= (seq_t)g;
        den /= g;
        num /= den;
        r *= (seq_t)num;
    }
    return r;
}
