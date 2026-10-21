/*
 * A224701  Table read by antidiagonals of numbers of form (2^n - 1)*2^(m+3) +
 *          5 where n>=1, m>=1.
 * https://oeis.org/A224701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^i - 1) * 2^(j+3) + 5, where (i,j) are the row and
 *            column * indices in the table read along antidiagonals: (i,j) =
 *            (1,1),(1,2),(2,1),... * and i+j = k with k the minimal integer
 *            satisfying n ≤ (k-1)*k/2
 * Range:     u8 n <= 10, u16 n <= 78, u32 n <= 406, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for unexpected n=0 */

    /* Find the diagonal k such that (k-1)*k/2 ≥ n */
    uint64_t t   = 1ULL + 8ULL * (uint64_t)n;
    double   d   = sqrt((double)t);
    idx_t    k   = (idx_t)((d + 1.0) / 2.0);   /* floor of (1+√t)/2   */
    idx_t    tri = (k - 1) * k / 2;            /* T(k) */
    if (tri < n) k++;                          /* adjust for ceil     */

    /* Determine position within diagonal k */
    idx_t prev   = (k - 2) * (k - 1) / 2;      /* T(k-1) */
    idx_t off    = n - prev - 1;               /* 0‑based offset     */
    idx_t i      = off + 1;                    /* row index          */
    idx_t j      = k - i;                      /* column index       */

    /* Compute the table entry and add 5 */
    seq_t row = pow2(i);          /* 2^i */
    row -= 1;
    seq_t col = pow2(j + 3);      /* 2^(j+3) */
    seq_t res = row * col;
    res += 5;
    return res;
}
