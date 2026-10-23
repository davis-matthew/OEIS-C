/*
 * A131504  Array read by antidiagonals: a(m,n) = m-th integer from among
 *          those positive integers that are coprime to (m*n).
 * https://oeis.org/A131504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals from the 2‑D array * a(m,n) = the
 *            m‑th integer coprime to m*n. * For a given linear index n
 *            (1‑based) we locate the pair (m,n) * by solving (k-1)*k/2 < n ≤
 *            k*(k+1)/2 where k = m+n-1, * then we compute the m‑th coprime
 *            value of (m*n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find k such that k*(k+1)/2 >= n   (k = m+n-1) */
    idx_t k = 0;
    /* Use 128‑bit arithmetic to avoid overflow */
    while (((unsigned __int128)k * (k + 1)) / 2 < n)
        ++k;

    /* Number of elements before this diagonal */
    idx_t prev = (k > 0) ? ((k - 1) * k) / 2 : 0;

    /* Offset within the diagonal [0 .. k-1] */
    idx_t r = n - prev - 1;   /* r = 0 for the first element of the diag */

    /* Coordinates:
     *   m = r + 1
     *   n' = k - r          (since m + n' = k + 1)
     */
    idx_t m     = r + 1;
    idx_t ncol  = k - r;

    /* Product p = m * ncol  (may be larger than 64 bits) */
    seq_t p = (seq_t)m * (seq_t)ncol;

    /* Find the m‑th integer coprime to p */
    idx_t count = 0;
    for (seq_t x = 1;; ++x) {
        if (gcd(p, x) == 1) {
            ++count;
            if (count == m)
                return x;   /* found the desired term */
        }
    }
}
