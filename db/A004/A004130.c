/*
 * A004130  Numerators in expansion of (1-x)^{-1/4}.
 * https://oeis.org/A004130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = prod(k=1, n, (4k-3)/k * 2^A007814(k)), i.e. a(0) = 1,
 *            a(n) = a(n-1) * (4n-3) / oddpart(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t gcd_idx(idx_t x, idx_t y) {
    while (y != 0) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t r = 1;
    for (idx_t k = 1; k <= n; k++) {
        idx_t o = k;
        while ((o & 1) == 0) o >>= 1;
        idx_t m = 4 * k - 3;
        idx_t g;
        while (o > 1 && (g = gcd_idx(m, o)) > 1) {
            m /= g;
            o /= g;
        }
        /* now gcd(m, o) = 1, and o divides r * m, so o divides r */
        if (o > 1) r /= (seq_t)o;
        r *= (seq_t)m;
    }
    return r;
}
