/*
 * A004197  Triangle read by rows. T(n, k) = n - k if n - k < k, otherwise k.
 * https://oeis.org/A004197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   T(n,k) = min(k, n-k), triangle read by rows; a(n) = T(m, n -
 *            m*(m+1)/2) with m*(m+1)/2 <= n < (m+1)*(m+2)/2
 * Range:     u8 n <= 5049, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Validated: standard, pass, 124 terms through n = 4285 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t m = (isqrt_u64(8 * n + 1) - 1) / 2;
    while (m > 0 && m * (m + 1) / 2 > n) m--;
    while ((m + 1) * (m + 2) / 2 <= n) m++;
    uint64_t k = n - m * (m + 1) / 2;
    uint64_t r = m - k;
    return (seq_t)(k < r ? k : r);
}
