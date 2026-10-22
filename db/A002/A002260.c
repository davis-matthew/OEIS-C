/*
 * A002260  Triangle read by rows: T(n,k) = k for n >= 1, k = 1..n.
 * https://oeis.org/A002260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl, look
 * Formula:   a(n) = n - m*(m+1)/2, where m = floor((sqrt(8*n-7) - 1)/2)
 * Range:     u8 n <= 8934, u16 n <= 8934, u32 n <= 8934, u64 n <= 8934,
 *            u128 n <= 8934, big any n
 * Helpers:   a002260_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002260_isqrt.h"

seq_t a(idx_t n) {
    uint64_t m = (a002260_isqrt(8 * (uint64_t)n - 7) - 1) / 2;
    /* adjust so that m*(m+1)/2 < n <= (m+1)*(m+2)/2 */
    while (m > 0 && m * (m + 1) / 2 >= n) m--;
    while ((m + 1) * (m + 2) / 2 < n) m++;
    return (seq_t)(n - m * (m + 1) / 2);
}
