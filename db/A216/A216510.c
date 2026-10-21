/*
 * A216510  Number of positive integer solutions to the equation a^2 + 6*b^2 =
 *          n.
 * https://oeis.org/A216510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of positive integer solutions (a,b) to a^2 + 6*b^2
 *            = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t isqrt_u64(idx_t n) {
    /* integer square root of a 64 bit unsigned integer using the bitwise algorithm */
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* largest power of four <= 2^64-1 */

    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t cnt = 0;
    idx_t max_a = isqrt_u64(n);
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t a2 = a * a;
        idx_t r  = n - a2;
        if (r % 6u != 0u) continue;          /* b^2 must be integer */
        idx_t b2 = r / 6u;
        if (b2 == 0u) continue;              /* b must be positive */
        idx_t b = isqrt_u64(b2);
        if (b * b == b2) ++cnt;              /* found a valid (a,b) */
    }
    return cnt;
}
