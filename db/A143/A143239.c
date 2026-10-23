/*
 * A143239  Triangle read by rows, A126988 * A128407 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A143239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   mobius, upper_row_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A143239:
   Treated as a lower‑triangular matrix T(n,k) with
   T(n,k) = mu(k) * (n/k) if k divides n, else 0.
   The sequence a(n) is the triangle read by rows, so for the
   n‑th term we first locate the row m such that
   m(m+1)/2 >= n, then let k = n - (m-1)m/2 (1 ≤ k ≤ m).
   Finally a(n) = mu(k) * (m/k) if k | m, otherwise 0. */
#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/upper_row_bound.h"

/* Binary search to find the smallest m such that m*(m+1)/2 ≥ n.  */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = upper_row_bound(n);
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)  /* offset is 1 */
        return (seq_t)0;

    /* Step 1: find the row and column */
    idx_t m = find_row(n);                                         /* row number */
    idx_t prev_tri = (m - 1) * m / 2;                               /* (m-1)m/2 */
    idx_t k = n - prev_tri;                                         /* column (1‑based) */

    /* Step 2: compute the term */
    if (k == 0 || m % k != 0)                                      /* should not happen */
        return (seq_t)0;

    idx_t ratio = m / k;                                            /* m/k fits in idx_t */
    int mob = (int)mobius(k);                                       /* mobius is -1,0,1 */

    if (mob == 0)
        return (seq_t)0;

    /* Convert to seq_t and apply sign */
    seq_t result = (seq_t)mob * (seq_t)ratio;
    return result;
}
