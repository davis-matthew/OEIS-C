/*
 * A164306  Triangle read by rows: T(n, k) = k / gcd(k, n), 1 <= k <= n.
 * https://oeis.org/A164306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k / gcd(k, r) where r and k satisfy n = r*(r-1)/2 + k (1
 *            ≤ k ≤ r)
 * Range:     u8 n <= 45, u16 n <= 5422, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reads the triangle T(n,k)=k/gcd(k,n) row-wise. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r-1) < n ≤ triangular(r) */
    idx_t low = 0, high = n;          /* high is inclusive bound for r */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;                     /* r is the row number (1‑based) */

    /* Compute k = column index within that row */
    seq_t prev_tri = (seq_t)r * ((seq_t)(r - 1)) / 2; /* triangular(r-1) */
    idx_t k = (idx_t)((seq_t)n - prev_tri);          /* 1 ≤ k ≤ r */

    /* Return T(r,k) = k / gcd(k, r) */
    seq_t g = gcd((seq_t)k, (seq_t)r);
    return (seq_t)k / g;
}
