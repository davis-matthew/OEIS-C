/*
 * A176270  Triangle T(n,m) = 1 + m*(m-n) read by rows, 0 <= m <= n.
 * https://oeis.org/A176270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = k^2 - r*k + 1 where r = floor((sqrt(8*n + 1) - 1) / 2) k
 *            = n - r*(r + 1)/2 (This computes the triangle T(r, k) = 1 + k*(k
 *            - r) read by rows.)
 * Range:     i8 n <= 31, i16 n <= 716, i32 n <= 5150, i64 n <= 5150,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    // Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
    idx_t low = 0;
    idx_t high = n;          // Upper bound inclusive
    idx_t r = 0;
    seq_t target = (seq_t)n; // Convert for comparison

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = ((seq_t)mid * ((seq_t)mid + 1)) / 2;

        if (tri <= target) {
            r = mid;          // mid is a candidate for r
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Compute k = n - r*(r+1)/2 */
    seq_t tri_r = ((seq_t)r * ((seq_t)r + 1)) / 2;
    seq_t diff = target - tri_r; // diff fits in the range [0, r]
    idx_t k = (idx_t)diff;

    /* Evaluate T(r, k) = k^2 - r*k + 1 */
    seq_t val = ((seq_t)k * k) - ((seq_t)r * k) + 1;
    return val;
}
