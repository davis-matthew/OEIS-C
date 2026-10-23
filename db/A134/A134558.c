/*
 * A134558  Array read by antidiagonals, a(n,k) = gamma(n+1,k)*e^k, where
 *          gamma(n,k) is the upper incomplete gamma function and e is the
 *          exponential constant 2.71828...
 * https://oeis.org/A134558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry read by anti‑diagonals from the array * a(n,k)
 *            defined by * a(0,k) = 1 * a(n,k) = n*a(n-1,k) + k^n (n>0) * The
 *            sequence A134558 lists these values with offset 0
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types used by the build */

seq_t a(idx_t n)
{
    /* Find the antidiagonal index d such that
     *   d*(d+1)/2 <= n < (d+1)*(d+2)/2.   */
    idx_t d = 0;
    /* small n guarantees that d stays modest (≤ √(2n)) */
    while ((d * (d + 1)) / 2 <= n)
        ++d;
    --d;   /* last d that satisfies the inequality */

    idx_t tot_before = (d * (d + 1)) / 2;   /* number of elements before this diagonal */
    idx_t t  = n - tot_before;             /* position within the diagonal */
    idx_t k  = t;                           /* column index (k)      */
    idx_t row = d - t;                      /* row index (n)         */

    /* Compute a(row,k) using the recurrence.
     * Start with a(0,k)=1, then iterate i=1..row.   */
    seq_t res   = 1;      /* a(0,k) */
    seq_t powk  = 1;      /* k^0  */

    for (idx_t i = 1; i <= row; ++i) {
        /* multiply by k each step to obtain k^i */
        powk = powk * (seq_t)k;          /* k^i  */
        /* a(i,k) = i * a(i-1,k) + k^i  */
        res = (seq_t)i * res + powk;
    }

    return res;
}
