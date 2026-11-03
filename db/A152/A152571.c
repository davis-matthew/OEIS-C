/*
 * A152571  Triangle T(n,k) read by rows: T(n,n) = -1, T(n,0) = 4^(n - 1),
 *          T(n,k) = -4^(n - k - 1), 1 <= k <= n - 1.
 * https://oeis.org/A152571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 44, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152571: triangle T(n,k) read by rows.
   a(0) = -1
   For n > 0, let r = floor((sqrt(1+8*n)-1)/2)
   k = n - r*(r+1)/2
   If k == 0        -> a = 4^(r-1)
   If k == r        -> a = -1
   otherwise        -> a = -4^(r-k-1)  */
#include <stdint.h>
#include <math.h>

/* small helper: compute 4^e as seq_t */
static seq_t pow4_small(idx_t e)
{
    seq_t v = 1;
    for (idx_t i = 0; i < e; ++i)
        v *= (seq_t)4;
    return v;
}

seq_t a(idx_t n)
{
    /* handle first element independently */
    if (n == 0)
        return -1;

    /* find the row r such that offset_row(r) <= n < offset_row(r+1) */
    double sq = sqrt((double)(8 * n + 1));
    idx_t r = (idx_t)((sq - 1.0) / 2.0);
    /* correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;            /* position within the row */

    if (k == 0)
        return pow4_small(r - 1);    /* T(r,0) = 4^(r-1)   */
    if (k == r)
        return -1;                  /* T(r,r) = -1        */
    return -(pow4_small(r - k - 1)); /* T(r,k) = -4^(r-k-1) */
}
