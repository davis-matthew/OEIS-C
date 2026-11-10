/*
 * A056537  Mapping from the column-by-column reading to the half-antidiagonal
 *          reading of the triangular tables. Inverse of A056536.
 * https://oeis.org/A056537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(j, m+1-j) where m is the least integer with m(m+1)/2 >=
 *            n, j = n - m(m-1)/2, and t(r,k) = (r-1)*(r+k) + floor(k^2/4) + 1
 * Range:     u8 n <= 135, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Validated: standard, pass, 87 terms through n = 4811 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* find m: smallest m with m(m+1)/2 >= n */
    idx_t m = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    if (m < 1) m = 1;
    while (m > 1 && (m - 1) * m / 2 >= n) m--;
    while (m * (m + 1) / 2 < n) m++;
    idx_t j = n - m * (m - 1) / 2;   /* row, 1..m */
    idx_t k = m + 1 - j;             /* column */
    seq_t r = (seq_t)j;
    seq_t c = (seq_t)k;
    return (r - 1) * (r + c) + (c * c) / 4 + 1;
}
