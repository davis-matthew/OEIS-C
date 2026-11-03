/*
 * A001966  v-pile counts for the 4-Wythoff game with i=2.
 * https://oeis.org/A001966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1/2)*(3+sqrt 5)); with m = 2n+1 this is a(n) = m
 *            + floor((m + floor(sqrt(5*m^2)))/2)
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9355 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a001966_isqrt_seq(seq_t x, seq_t start)
{
    seq_t r = start, y;
    if (r == 0) return r;
    y = (r + x / r) / 2;
    while (y < r) {
        r = y;
        y = (r + x / r) / 2;
    }
    return r;
}

static uint64_t a001966_isqrt_u64(uint64_t x, uint64_t start)
{
    /* Newton from an upper bound start >= sqrt(x) */
    uint64_t r = start, y;
    if (r == 0) return 0;
    y = (r + x / r) / 2;
    while (y < r) {
        r = y;
        y = (r + x / r) / 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < ((idx_t)1 << 29)) {
        uint64_t m = 2 * (uint64_t)n + 1;
        uint64_t s = a001966_isqrt_u64(5 * m * m, 3 * m);
        uint64_t v = m + (m + s) / 2;
        return (seq_t)v;
    } else {
        seq_t m = (seq_t)n * 2 + 1;
        seq_t s = a001966_isqrt_seq(m * m * 5, m * 3);
        return m + (m + s) / 2;
    }
}
