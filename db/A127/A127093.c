/*
 * A127093  Triangle read by rows: T(n,k)=k if k is a divisor of n; otherwise,
 *          T(n,k)=0 (1 <= k <= n).
 * https://oeis.org/A127093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (k divides r) ? k : 0 * where * r = ceil((sqrt(8*n + 1) -
 *            1) / 2) (row index) * k = n - r*(r-1)/2 (column index) * a(n) =
 *            k if r % k == 0, else 0 * This implements OEIS A127093, the
 *            flattened divisor triangle
 * Range:     u8 n <= 9883, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Compute the row index r for a given flattened sequence index n */
static inline idx_t _row_of(idx_t n)
{
    if (n == 0) return 0;
    /* Approximate r using floating point sqrt, then adjust */
    long double root = sqrt((long double)8.0L * n + 1.0L);
    idx_t r = (idx_t)((root - 1.0L) / 2.0L);
    /* Ensure r is the smallest integer with r*(r+1)/2 >= n */
    while (((__int128)r * (r + 1)) / 2 < n) ++r;
    while (r > 0 && ((((__int128)(r - 1) * r) / 2) >= n)) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t r = _row_of(n);
    /* k = position within row r */
    idx_t prev_tri = (__int128)r * (r - 1) / 2;   /* triangular(r-1) */
    idx_t k = n - prev_tri;
    /* Return k if k divides r, else 0 */
    return (r % k == 0) ? (seq_t)k : (seq_t)0;
}
