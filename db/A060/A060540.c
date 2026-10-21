/*
 * A060540  Square array read by antidiagonals downwards: T(n,k) =
 *          (n*k)!/(k!^n*n!), (n>=1, k>=1), the number of ways of dividing nk
 *          labeled items into n unlabeled boxes with k items in each box.
 * https://oeis.org/A060540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 39, u64 n <= 59, u128 n <= 97,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A060540 – Square array read by antidiagonals (downwards)
   Formula: a(n) = T(i,j) where
              T(n,k) = (n*k)! / (k!^n * n!)
              (i,j) are the row/column indices obtained from the
              n-th element of the array read along antidiagonals:
              find the smallest d with d*(d+1)/2 ≥ n
              let pos = n - (d-1)*d/2
              then i = pos,   j = d - pos + 1
*/

#include <stdint.h>
#include "../utils/binomial.h"

/* compute T(n,k) = (n*k)! / (k!^n * n!) by the product formula: */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n <= 1 || k <= 1) return 1;          /* T(1,*) = T(*,1) = 1 */

    seq_t result = 1;
    for (idx_t t = 2; t <= n; ++t) {
        seq_t bin = binomial(t * k - 1, k - 1); /* binom(t*k-1, k-1) */
        result *= bin;                         /* sequential multiply */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0 */

    /* binary search for the diagonal d such that d*(d+1)/2 ≥ n */
    idx_t low  = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                          /* diagonal index (≥1) */

    /* number of terms before this diagonal */
    unsigned __int128 prev = (unsigned __int128)(d - 1) * d / 2;
    idx_t pos = n - (idx_t)prev;            /* 1‑based position within diagonal */

    /* coordinates (i,j) on this diagonal: i = pos, j = d - pos + 1 */
    idx_t i = pos;
    idx_t j = d - pos + 1;

    return compute_T(i, j);
}
