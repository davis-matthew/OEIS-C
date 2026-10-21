/*
 * A078436  Triangle read by rows in which n-th row counts multisets
 *          associated with hook partitions.
 * https://oeis.org/A078436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle whose entries are
 *            T(r,k) = (r - k + 1) * 2^(k-1) for 1 ≤ k < r, and T(r,r) = 0.
 *            The only special case is r = 1, where T(1,1) = 1. The flattened
 *            sequence is read row by row
 * Range:     u8 n <= 43, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* row 1, column 1 */

    /* Find the row r such that r*(r+1)/2 ≥ n. */
    long double d = sqrtl((long double)8 * n + 1);
    idx_t r = (idx_t)((d - 1) / 2);
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* Column index k (1‑based) within the row. */
    idx_t k = n - (unsigned __int128)(r - 1) * r / 2;

    if (k == r) return (seq_t)0;          /* last entry of the row */

    /* Main formula: (r - k + 1) * 2^(k-1). */
    return (seq_t)(r - k + 1) * pow2(k - 1);
}
