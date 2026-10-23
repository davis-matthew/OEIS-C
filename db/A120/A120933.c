/*
 * A120933  Triangle read by rows: T(n,k) is the number of binary words of
 *          length n for which the length of the maximal leading nondecreasing
 *          subword is k (1<=k<=n).
 * https://oeis.org/A120933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(n) = T(r,k) where
   r = floor((sqrt(8*(n-1)+1)+1)/2),
   k = n - r*(r-1)/2;
   T(r,k) = k*2^(r-k-1) for k<r, otherwise r+1. */

#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* zero‑based linear index */
    idx_t L = n - 1U;

    /* approximate the row number using the quadratic formula */
    double approx = sqrt(8.0 * (double)L + 1.0);
    idx_t r = (idx_t)((approx + 1.0) * 0.5);

    /* Adjust R to satisfy r(r-1)/2 <= L < r(r+1)/2 */
    unsigned __int128 start, next;
    while ((start = (unsigned __int128)(r - 1) * r / 2U) > L) r--;
    while ((next = (unsigned __int128)r * (r + 1) / 2U) <= L) r++;

    /* position within the row */
    start = (unsigned __int128)(r - 1) * r / 2U;
    idx_t k = (idx_t)(L - start) + 1U;   /* columns are 1‑based */

    /* compute the triangle entry */
    if (k < r)
        return (seq_t)k * pow2(r - k - 1U);
    else
        return (seq_t)(r + 1U);
}
