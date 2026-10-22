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
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    uint64_t m = (a000037_isqrt(8 * n + 1) - 1) / 2;
    while (m > 0 && m * (m + 1) / 2 > n) m--;
    while ((m + 1) * (m + 2) / 2 <= n) m++;
    uint64_t k = n - m * (m + 1) / 2;
    uint64_t r = m - k;
    return (seq_t)(k < r ? k : r);
}
