/*
 * A100791  Group the natural numbers so that the n-th group contains n(n+1)/2
 *          = T(n) terms: (1), (2,3,4), (5,6,7,8,9,10),
 *          (11,12,13,14,15,16,17,18,19,20),(21,22,23,24,25,26,27,28,29,30,31,32,33,34,35),...
 *          The n-th row of the following triangle is formed from the sum of
 *          first n terms, next n-1 terms,next n-2...
 * https://oeis.org/A100791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the sum of a consecutive block of length (r‑s) in the
 *            r‑th group of natural numbers, where r = minimal integer with
 *            r(r+1)/2 ≥ n (row number) s = n − r(r−1)/2 − 1 (position within
 *            the row)
 * Range:     u8 n <= 21, u16 n <= 325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find r: minimal number with r(r+1)/2 ≥ n */
    idx_t low = 1, high = n, r = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            r = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* offset within the row */
    unsigned __int128 before_rows = (unsigned __int128)(r - 1) * r / 2;
    idx_t s = n - (idx_t)before_rows - 1;

    /* start number of r‑th group: 1 + sum_{k=1}^{r-1} T(k) = 1 + (r-1)r(r+1)/6 */
    unsigned __int128 S_prev = (unsigned __int128)(r - 1) * r * (r + 1) / 6;
    seq_t N_start = (seq_t)S_prev + 1;

    /* first number of the block */
    seq_t rr = (seq_t)r;
    seq_t ss = (seq_t)s;
    seq_t a_start = N_start + ss * rr - ss * (ss - 1) / 2;

    /* block length */
    seq_t L = rr - ss;

    /* block sum: (first + last) * L / 2 = (2*a_start + L - 1)*L/2 */
    seq_t sum = ((a_start * 2 + L - 1) * L) / 2;
    return sum;
}
