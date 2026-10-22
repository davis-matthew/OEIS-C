/*
 * A006273  Numerators of a continued fraction for (3+sqrt(13))/2.
 * https://oeis.org/A006273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = a(n-1)^3 + 3*a(n-1)^2 - 3 for n >= 2, a(0) = 3, a(1) = 10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t x;
    idx_t i;
    if (n == 0) {
        x = 3;
        return x;
    }
    x = 10;
    for (i = 2; i <= n; i++) {
        seq_t sq = x * x;
        x = sq * x + 3 * sq - 3;
    }
    return x;
}
