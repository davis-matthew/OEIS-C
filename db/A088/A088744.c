/*
 * A088744  a(n) = 3*A088023(n) - 2.
 * https://oeis.org/A088744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*b(n) - 2, where b = A088023: b(1)=1, b(2)=2; with t =
 *            2^floor(log2 n): b(t) = 2*b(t/2) - 1 for t >= 4, and b(n) =
 *            b(2t-n+1) + (n-t) for n > t
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9931 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A088023_a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    idx_t t = 1;
    while (t <= n / 2) t <<= 1;
    if (n == t) {
        return A088023_a(t >> 1) * 2 - 1;
    }
    idx_t m = (t << 1) - n + 1;
    return A088023_a(m) + (seq_t)(n - t);
}

seq_t a(idx_t n) {
    return A088023_a(n) * 3 - 2;
}
