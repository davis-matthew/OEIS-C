/*
 * A001805  a(n) = n! * binomial(n,3).
 * https://oeis.org/A001805
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * binomial(n,3), with binomial(n,3) = n*(n-1)*(n-2)/6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n, y = x - 1, z = x - 2;
    /* divide out 2 and 3 from the factors before multiplying, so the product stays small */
    if (x % 2 == 0) x /= 2; else y /= 2;
    if (x % 3 == 0) x /= 3; else if (y % 3 == 0) y /= 3; else z /= 3;
    seq_t r = (seq_t)x;
    r *= (seq_t)y;
    r *= (seq_t)z;
    for (idx_t i = 2; i <= n; i++) {
        r *= (seq_t)i;
    }
    return r;
}
