/*
 * A195040  Square array read by antidiagonals with T(n,k) =
 *          k*n^2/4+(k-4)*((-1)^n-1)/8, n>=0, k>=0.
 * https://oeis.org/A195040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the diagonal read of the square array * T(n,k) = k*n^2/4
 *            + (k-4)*((-1)^n-1)/8 for n,k >= 0. * The sequence is produced by
 *            reading T(n,k) with * n+k increasing and, for each diagonal, n
 *            decreasing * (i.e., k increasing from 0). The offset is 0. *
 *            This implementation uses integer arithmetic...
 * Range:     u8 n <= 213, u16 n <= 4949, u32 n <= 4949, u64 n <= 4949,
 *            u128 n <= 4949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Compute the entry T(n,k) using a single rational expression.
 *  T(n,k) = (2*k*n^2 + (k-4)*((-1)^n-1))/8
 * The numerator is always a multiple of 8, so the division is exact.
 */
static inline seq_t T_val(idx_t n, idx_t k)
{
    // Use signed 128‑bit for intermediate calculation
    __int128 num = (__int128)2 * k * n * n;          // 2*k*n^2
    if (n & 1) {                                     // n odd
        __int128 diff = (__int128)k - 4;              // k - 4
        num += -(__int128)2 * diff;                  // + (k-4)*(-2)
    }
    /* now num is divisible by 8 */
    return (seq_t)(num / 8);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal index d (called s below) such that
     *   d*(d+1)/2 <= n < (d+1)*(d+2)/2 .
     */
    idx_t d;
    if (n == 0) {
        d = 0;
    } else {
        long double tmp = sqrtl((long double)8.0L * n + 1.0L);
        d = (idx_t)((tmp - 1.0L) / 2.0L);          /* initial guess */
    }

    /* Correct any off‑by‑one caused by the floating point estimate. */
    while ((__int128)d * (d + 1) / 2 > n) d--;
    while ((__int128)(d + 1) * (d + 2) / 2 <= n) d++;

    /* Offset to the first index of diagonal d */
    idx_t prev = (idx_t)((__int128)d * (d + 1) / 2);
    idx_t pos   = n - prev;           /* position on diagonal, 0 <= pos <= d */
    idx_t n_val = d - pos;            /* row index */
    idx_t k_val = pos;                /* column index */

    return T_val(n_val, k_val);
}
