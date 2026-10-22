/*
 * A004303  a(n) = binomial(2*n-2,n-1)/n - 2^(n-1) + n.
 * https://oeis.org/A004303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-2,n-1)/n - 2^(n-1) + n, i.e. a(n) = C(n-1) +
 *            n - 2^(n-1) with Catalan C(0) = 1, C(k+1) = C(k)*2*(2k+1)/(k+2)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

static seq_t seq_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 0; k + 2 <= n; k++) {
        seq_t num = (seq_t)2 * (seq_t)(2 * k + 1);
        seq_t den = (seq_t)(k + 2);
        seq_t g = seq_gcd(c % den, den);
        if (g == 0) g = den;
        seq_t d = den / g;
        c = (c / g) * (num / d);
    }
    seq_t result = c + (seq_t)n;
    result -= A001832_pow2(n - 1);
    return result;
}
