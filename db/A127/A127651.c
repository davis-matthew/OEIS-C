/*
 * A127651  Triangle T(n,k) = n*k if k|n, 0 otherwise; 1<=k<=n.
 * https://oeis.org/A127651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (row, column) entry of the triangle T(r,k) = r*k if
 *            k | r else 0, where r is the smallest integer with r(r+1)/2 ≥ n
 *            and k = n – r(r-1)/2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for n=0 */
    idx_t r = 0;
    /* find smallest r such that r(r+1)/2 ≥ n */
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev = (r - 1) * r / 2;        /* entries before row r */
    idx_t k = n - prev;                  /* column within row r */

    if (r % k == 0) {
        return (seq_t)r * (seq_t)k;      /* divisible: output product */
    } else {
        return (seq_t)0;                 /* otherwise 0 */
    }
}
