/*
 * A004988  a(n) = (3^n/n!) * Product_{k=0..n-1} (3*k + 2).
 * https://oeis.org/A004988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n-1)*a(n-1)/n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        uint64_t mult = 3 * (3 * (uint64_t)i - 1);
        uint64_t r = (uint64_t)(result % i);
        uint64_t g = gcd_u64(i, r);
        uint64_t d = i / g;
        result = result / g;
        result = result * (mult / d);
    }
    return result;
}
