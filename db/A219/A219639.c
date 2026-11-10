/*
 * A219639  Numbers that occur only once in A219641.
 * https://oeis.org/A219639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A219641(A035336(n)), where A035336(n) = 2*floor(n*phi) +
 *            n - 1 and A219641(m) = m - (number of 1's in the Zeckendorf
 *            expansion of m)
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9002 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t A219639_m(idx_t n)
{
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 lo = 2 * N, hi = 3 * N, target = 5 * N * N;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= target) lo = mid; else hi = mid - 1;
    }
    unsigned __int128 f = (N + lo) / 2;
    return (idx_t)(2 * f + N - 1);
}

seq_t a(idx_t n)
{
    static uint64_t fibs[100];
    static size_t len = 0;
    if (len == 0) {
        fibs[0] = 1; fibs[1] = 2; len = 2;
        while (len < 100) {
            uint64_t nx = fibs[len - 1] + fibs[len - 2];
            if (nx < fibs[len - 1]) break;
            fibs[len++] = nx;
        }
    }
    idx_t m = A219639_m(n);
    idx_t rem = m;
    seq_t ones = 0;
    size_t i = len;
    while (rem > 0 && i > 0) {
        i--;
        if (fibs[i] <= rem) {
            rem -= fibs[i];
            ones++;
        }
    }
    return (seq_t)m - ones;
}
