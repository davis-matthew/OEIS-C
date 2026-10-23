/*
 * A126277  Triangle generated from Eulerian numbers.
 * https://oeis.org/A126277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^k - k - 1)*(r - k) + k where r = smallest integer with
 *            r(r+1)/2 ≥ n, and k = n - r(r-1)/2 (row and column in the
 *            triangle) The triangle entry is T(r,k) = Eulerian( k-1, k-2
 *            )*(r-k) + k, with Eulerian( k-1, k-2 ) = 2^k - k - 1. This
 *            implements a(idx_t n) for OEIS A126277
 * Range:     u8 n <= 51, u16 n <= 167, u32 n <= 591, u64 n <= 2207,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row 'r' containing the n-th element. */
    idx_t low  = 1;
    idx_t high = 1;
    /* Increase high until sum(high) >= n */
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* row index (1‑based) */
    idx_t prev = (r - 1) * r / 2;         /* elements before this row */
    idx_t k = n - prev;                  /* column index within the row (1‑based) */

    /* Compute 2^k as a seq_t using repeated doubling. */
    seq_t pow2k = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2k += pow2k;                  /* doubling: pow2k = pow2k * 2 */

    /* Eulerian( k-1, k-2 ) = 2^k - k - 1 */
    seq_t diff = pow2k - (seq_t)k - 1;

    /* T(r,k) = diff * (r - k) + k */
    seq_t val = diff * (seq_t)(r - k) + (seq_t)k;
    return val;
}
