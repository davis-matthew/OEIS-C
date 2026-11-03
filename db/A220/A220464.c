/*
 * A220464  Reverse reluctant sequence of reluctant sequence A002260.
 * https://oeis.org/A220464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = A002260(A004736(n)) where A002260(m) = m - T_{k-1} with
 *            T_k = k*(k+1)/2 and k = ceil( (sqrt(8m+1)-1)/2 ). A004736(n)
 *            gives the index m of the n‑th term in the reverse reluctant
 *            triangle. The algorithm below finds the row r in the reverse
 *            triangle, the position within that row, determines t...
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 136 terms through n = 9385 (2026-11-01)
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/triangular.h"

/* Binary search to find the minimal r such that T(r) >= n (r >= 1). */
static inline idx_t ceil_tri_row_u64(idx_t n)
{
    idx_t low = 1;
    /* Choose a high that exceeds sqrt(2*max_n).  1<<33 is > 2^32,
       thus sufficient for all 64‑bit indices. */
    idx_t high = (idx_t)1ULL << 33;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Compute T_{r-1} = r*(r-1)/2 safely. */
static inline idx_t tri_prev_u64(idx_t r)
{
    return (idx_t)(((__int128)r * (r - 1)) / 2);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)  /* out of offset, return 0 as a safeguard */
        return 0;

    /* Find row r of the reverse reluctant triangle that contains n.
       r is the smallest integer with T(r) >= n. */
    idx_t r = ceil_tri_row_u64(n);
    idx_t prev_r = tri_prev_u64(r);        /* T(r-1) */
    idx_t pos_in_row = n - prev_r;         /* 1‑based position within the row */
    idx_t m = r - pos_in_row + 1;          /* index into A002260 */

    /* Compute A002260(m) = m - T(k-1) where k is the row number of index m. */
    idx_t k = ceil_tri_row_u64(m);
    idx_t prev_k = tri_prev_u64(k);
    seq_t result = (seq_t)(m - prev_k);

    return result;
}
