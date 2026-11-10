/*
 * A088743  a(n) = 2*A088023(n) - 1.
 * https://oeis.org/A088743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A088023(n) - 1; A088023: a(1)=1, a(2)=2; t=2^floor(log2
 *            n); if n=t>=4 then a(n)=2*a(n/2)-1, else a(n)=a(2t-n+1)+(n-t)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9464 (2026-11-07)
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
    idx_t m = 2 * t - n + 1;
    return A088023_a(m) + (seq_t)(n - t);
}

seq_t a(idx_t n) {
    return A088023_a(n) * 2 - 1;
}
