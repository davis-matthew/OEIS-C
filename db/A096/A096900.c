/*
 * A096900  a(n) = (A088558(n)-1)/2.
 * https://oeis.org/A096900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A088558(n)-1)/2, where A088558(n) = min over coprime
 *            opposite-parity factor pairs m*k = 2n, m > k, of m^2 - k^2
 *            (divisor search)
 * Range:     u8 n <= 8, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A096900_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t N = (uint64_t)n;
    uint64_t P = 2 * N;
    seq_t best = 0;
    int first = 1;
    for (uint64_t d = 1; d * d <= P; ++d) {
        if (P % d != 0) continue;
        uint64_t m = P / d;
        if (m <= d) continue;
        if (A096900_gcd(m, d) != 1) continue;
        if ((m & 1ULL) == (d & 1ULL)) continue;
        seq_t leg = (seq_t)m * (seq_t)m - (seq_t)d * (seq_t)d;
        if (first || leg < best) {
            best = leg;
            first = 0;
        }
    }
    return (best - 1) / 2;
}
