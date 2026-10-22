/*
 * A004354  Binomial coefficient C(5n, n-12).
 * https://oeis.org/A004354
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(5n, n-12), computed multiplicatively with gcd reduction
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 18, u64 n <= 26, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t g_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n) {
    if (n < 12) return 0;
    idx_t m = 5 * n;
    idx_t k = n - 12;
    if (k > m - k) k = m - k;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = C(m-k+i-1, i-1); next r = r * (m-k+i) / i */
        uint64_t num = (uint64_t)(m - k + i);
        uint64_t den = (uint64_t)i;
        uint64_t rm = (uint64_t)(r % (seq_t)den);
        uint64_t g = g_gcd(den, rm);   /* gcd(r, den) */
        den /= g;
        r /= (seq_t)g;
        /* den now divides num */
        num /= den;
        r *= (seq_t)num;
    }
    return r;
}
