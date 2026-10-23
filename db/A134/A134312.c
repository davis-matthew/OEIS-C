/*
 * A134312  A097806 * A134309.
 * https://oeis.org/A134312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row,col) entry of the infinite lower triangular
 *            matrix defined by A097806 * A134309. For row ≥ 2 and column j
 *            (0‑based) in that row: if j < row‑1 → 0 else → 2^(j‑1) For row 0
 *            or 1 all entries are 1. The values are indexed row-wise (row 0
 *            first, then row 1, …)
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/pow2.h"

/* Helper: triangular number of a row.
   T(r) = number of entries up to and including row r
   = (r+1)(r+2)/2. */
static inline idx_t T_row(idx_t r)
{
    __int128 t = (__int128)(r + 1) * (r + 2);
    t >>= 1;                     /* divide by 2 */
    return (idx_t)t;
}

/* Find the row index r such that 0 ≤ n < T(r).  */
static inline idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* grow high until T(high) > n */
    while (T_row(high) <= n) {
        if (high > UINT64_MAX / 2) {    /* prevent overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    /* binary search between low and high (high is the first satisfying row) */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (T_row(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    return high;   /* first row with T(r) > n */
}

seq_t a(idx_t n)
{
    /* Locate the row and column for n */
    idx_t row   = find_row(n);
    idx_t tprev = (row == 0) ? 0 : T_row(row - 1);
    idx_t col   = n - tprev;   /* 0‑based column within that row */

    /* Special case: rows 0 and 1 all entries are 1 */
    if (row <= 1) {
        return (seq_t)1;
    }

    /* For rows ≥ 2 */
    if (col < row - 1) {
        return (seq_t)0;
    } else {
        /* col ≥ row‑1, thus col ≥ 1; exponent = col‑1 */
        return pow2(col - 1);
    }
}
