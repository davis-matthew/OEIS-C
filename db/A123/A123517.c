/*
 * A123517  Triangle read by rows: T(n,k) = floor(n/k + 1/2) - floor(n/(k +
 *          1/2)) (1<=k<=n).
 * https://oeis.org/A123517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangle * T(r,k) = ⌊r/k + ½⌋ −
 *            ⌊r/(k + ½)⌋, * read by rows: first row r=1 has length 1, second
 *            row r=2 has length 2, etc. * For a given n (1‑based) we find the
 *            unique row r such that * ½ r (r−1) < n ≤ ½ r (r+1), * set k = n
 *            − ½ r (r−1), and return T(r,k)
 * Range:     u8 n <= 44, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* find row r via binary search on the inequality
     * r*(r+1)/2 >= n
     */
    idx_t lo = 1, hi = n, r = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t sum = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (sum < n) {
            lo = mid + 1;
        } else {
            r = mid;
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    /* previous total entries before row r */
    seq_t prev_total = ((seq_t)(r - 1) * (seq_t)r) / 2;
    idx_t k = n - (idx_t)prev_total;            /* column index, 1‑based */

    /* Compute T(r,k) = floor((2r + k)/(2k)) − floor((2r)/(2k+1)) */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;

    seq_t t1 = (2 * rr + kk) / (2 * kk);          /* floor(r/k + ½)   */
    seq_t t2 = (2 * rr) / (2 * kk + 1);           /* floor(r/(k+½))   */

    return t1 - t2;
}
