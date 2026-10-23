/*
 * A112333  An invertible triangle of ratios of triple factorials.
 * https://oeis.org/A112333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Π_{j=k+1}^{r} (3*j-1) where * r is the row index with
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2, * k = n - r(r+1)/2 is the column
 *            offset within the row
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Determine the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
   For the ranges of n we need (≤ 10 000) a simple linear search is fast enough. */
static inline idx_t triangle_row(idx_t n)
{
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) ++r;
    return r - 1;
}

seq_t a(idx_t n)
{
    idx_t r = triangle_row(n);
    idx_t k = n - r * (r + 1) / 2;
    seq_t prod = 1;
    for (idx_t j = k + 1; j <= r; ++j)
        prod *= (seq_t)(3 * j - 1);
    return prod;
}
