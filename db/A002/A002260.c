/*
 * A002260  Triangle read by rows: T(n,k) = k for n >= 1, k = 1..n.
 * https://oeis.org/A002260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl, look
 * Formula:   a(n) = n - m*(m+1)/2, where m = floor((sqrt(8*n-7) - 1)/2)
 * Range:     u8 n <= 11325, u16 n <= 11325, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 121 terms through n = 8934 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t m = (isqrt_u64(8 * (uint64_t)n - 7) - 1) / 2;
    /* adjust so that m*(m+1)/2 < n <= (m+1)*(m+2)/2 */
    while (m > 0 && m * (m + 1) / 2 >= n) m--;
    while ((m + 1) * (m + 2) / 2 < n) m++;
    return (seq_t)(n - m * (m + 1) / 2);
}
