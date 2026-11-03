/*
 * A004973  a(n) = ceiling(n*phi^18), where phi is the golden ratio, A001622.
 * https://oeis.org/A004973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^18) = 2889*n + ceiling(1292*n*sqrt(5)) =
 *            2889*n + floor(sqrt(5*(1292*n)^2)) + 1 for n >= 1, a(0) = 0
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static unsigned __int128 isqrt_u128(unsigned __int128 m)
{
    unsigned __int128 x, y;
    if (m < 2) return m;
    x = m;
    y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + m / x) / 2;
    }
    return x;
}

static seq_t isqrt_seq(seq_t m)
{
    seq_t x, y;
    if (m < 2) return m;
    x = m;
    y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + m / x) / 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t r;
    if (n == 0) return 0;
    if (n < (idx_t)1000000000000000ULL) {
        unsigned __int128 k = (unsigned __int128)n * 1292u;
        r = u128_to_seq(isqrt_u128(k * k * 5u));
    } else {
        seq_t k = (seq_t)n * 1292;
        r = isqrt_seq(k * k * 5);
    }
    return (seq_t)n * 2889 + r + 1;
}
