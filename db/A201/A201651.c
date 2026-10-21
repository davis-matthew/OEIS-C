/*
 * A201651  Bit-interleaved number addition table (cf. A054240) as triangle
 *          read by rows: T(n,k) = n and k added in binary representation,
 *          where carries shift 2 instead of 1, 0 <= k <= n.
 * https://oeis.org/A201651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k)
 * Range:     u8 n <= 2143, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   shifted_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = floor((sqrt(8*n+1)-1)/2),   */
/* k = n – r*(r+1)/2, and                */
/* T(r,k) is obtained by adding r and k   */
/* in binary with carries shifted two     */
/* positions instead of one.               */

#include <stdint.h>
#include "../utils/shifted_add.h"

/* Helper: compute (x * (x + 1) / 2) using 128‑bit arithmetic
   to avoid overflow for large r. */
static inline idx_t triangular(idx_t x)
{
    return (idx_t)(((unsigned __int128)x * (x + 1)) / 2);
}

/* Find the row r and column k that correspond to index n. */
static inline void rowcol_from_index(idx_t n, idx_t *r_out, idx_t *k_out)
{
    /* Binary search for the smallest r with r(r+1)/2 > n */
    idx_t low = 0;
    idx_t high = 1;
    while (triangular(high) <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (triangular(mid) <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    *r_out = low;
    *k_out = n - triangular(low);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r, k;
    rowcol_from_index(n, &r, &k);
    return shifted_add(r, k);
}
