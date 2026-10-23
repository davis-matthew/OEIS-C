/*
 * A172090  Triangle T(n, k) = f(n-k) + f(k) - f(n), where f(n) = -3*n with
 *          f(0) = 1, f(1) = -2, read by rows.
 * https://oeis.org/A172090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) is the nth element of triangle T read by rows; T(n,k) =
 *            f(n-k) + f(k) – f(n) where f(0)=1, f(1)=-2 and f(m)=-3*m for
 *            m>=2
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

static inline int64_t f_val(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -2;
    return -(int64_t)3 * (int64_t)n;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    uint64_t r;
    /* Quick initial guess with double arithmetic */
    double d = sqrt((double) (8.0 * (double)n + 1.0));
    r = (uint64_t)((d - 1.0) / 2.0);
    /* Adjust if needed (incoming n is modest, this loop is very fast) */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;

    /* Column k within that row */
    uint64_t k = n - r * (r + 1) / 2;

    /* Compute the triangle entry */
    int64_t val = f_val(r - k) + f_val(k) - f_val(r);
    return (seq_t)val;
}
