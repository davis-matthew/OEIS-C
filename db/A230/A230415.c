/*
 * A230415  Square array T(i,j) giving the number of differing digits in the
 *          factorial base representations of i and j, for i >= 0, j >= 0,
 *          read by antidiagonals.
 * https://oeis.org/A230415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   factorial_base_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A230415.
   a(n) is taken from the square array T(i,j) that gives the number of
   differing digits in the factorial‑base representations of i and j.
   The array is read by antidiagonals: for a given n,
   let k = max(d) such that k(k+1)/2 ≤ n.
   Then i = n - k(k+1)/2,  j = k - i, and a(n) = T(i,j).
   T(i,j) is computed by iterating over the factorial‑base digits
   (base = k+1, k starting at 1) and counting positions where the
   digits differ.
*/

#include <math.h>
#include "../utils/factorial_base_diff.h"

seq_t a(idx_t n)
{
    /* Find diagonal index k such that k(k+1)/2 ≤ n < (k+1)(k+2)/2 */
    idx_t k = 0;
    if (n > 0) {
        k = (idx_t)((sqrt((double)(8.0 * n + 1.0)) - 1.0) / 2.0);
        /* Adjust k up or down to satisfy the triangular inequality */
        while ((__uint128_t)(k + 1) * (k + 2) / 2 <= n) ++k;
        while ((__uint128_t)k * (k + 1) / 2 > n) --k;
    }
    idx_t diag_start = (__uint128_t)k * (k + 1) / 2;
    idx_t t = n - diag_start;      /* offset along the diagonal */
    idx_t i = t;
    idx_t j = k - t;
    return factorial_base_diff(i, j);
}
