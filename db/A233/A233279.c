/*
 * A233279  Permutation of nonnegative integers: a(n) = A054429(A006068(n)).
 * https://oeis.org/A233279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A054429(A006068(n)), a(0) = 0
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 84 terms through n = 7867 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t g = (seq_t)n;
    seq_t s = (seq_t)n;
    while (s > 0) {
        s >>= 1;
        g ^= s;
    }
    if (g == 0) return (seq_t)0;

    seq_t t = g;
    seq_t p = 1;
    while (t > 1) {
        t >>= 1;
        p <<= 1;
    }
    /* reverse within block [p, 2p-1] */
    return (p << 1) - 1 - (g - p);
}
