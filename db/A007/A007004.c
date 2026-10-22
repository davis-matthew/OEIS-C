/*
 * A007004  a(n) = (3*n)! / ((n+1)*(n!)^3).
 * https://oeis.org/A007004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(3*n-1)*(3*n-2)*a(n-1)/(n*(n+1)) for n>0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static idx_t gcd_u64(idx_t x, idx_t y) {
    while (y != 0) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        idx_t den = i * (i + 1);
        idx_t num = (idx_t)3 * (3 * i - 1) * (3 * i - 2);
        idx_t r = (idx_t)(long long)(v % den);
        idx_t g = gcd_u64(r, den);
        v /= g;
        den /= g;
        num /= den;
        v *= num;
    }
    return v;
}
