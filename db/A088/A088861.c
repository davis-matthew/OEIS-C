/*
 * A088861  Third column of triangle in A088643.
 * https://oeis.org/A088861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = third term of row n+2 of A088643 (greedy search: start
 *            N=n+2, then repeatedly take largest unused m<N with m+previous
 *            prime)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A088861_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n + 2;
    uint64_t b = 0, c = 0;
    for (uint64_t m = N - 1; m >= 1; --m) {
        if (A088861_isp(N + m)) { b = m; break; }
    }
    for (uint64_t m = N - 1; m >= 1; --m) {
        if (m != b && A088861_isp(b + m)) { c = m; break; }
    }
    return (seq_t)c;
}
