/*
 * A153271  Triangle T(n, k) = Product_{j=0..k} (j*n + prime(m)), with T(n, 0)
 *          = prime(m) and m = 3, read by rows.
 * https://oeis.org/A153271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 34, u64 n <= 76, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Triangle T(n,k) = ∏{j=0..k} (j*n + 5), with first prime p=5 (m=3). 
 * The sequence a(n) is formed by reading the rows of T(n,k) row‑by‑row:
 *     row 0 : T(0,0)
 *     row 1 : T(1,0), T(1,1)
 *     row 2 : T(2,0), T(2,1), T(2,2)
 *     ...
 * The function below computes a(n) for any n ≥ 0, using only integer
 * arithmetic and the allowed types seq_t and idx_t.
 */

#include <math.h>   /* for sqrt */

seq_t a(idx_t n)
{
    /* Determine which row r contains index n.
       The cumulative number of terms up to row r is (r+1)(r+2)/2.
       Solve r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.                     */
    double disc   = sqrt((double)8.0 * (double)n + 1.0);
    idx_t   r     = (idx_t)((disc - 1.0) / 2.0);

    /* Correct possible rounding errors of the floating point calculation. */
    while (((r + 1) * (r + 2)) / 2 <= n) r++;           /* increase if too low */
    while (r * (r + 1) / 2 > n) r--;                    /* decrease if too high */

    /* Index k within the row r: 0 ≤ k ≤ r. */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r,k) = ∏_{j=0..k} (j*r + 5).  Start with j=0 term = 5. */
    seq_t val = (seq_t)5;
    for (idx_t j = 1; j <= k; ++j) {
        val *= (seq_t)(j * r + 5);
    }

    return val;
}
