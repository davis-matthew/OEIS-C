/*
 * A077028  The rascal triangle, read by rows: T(n,k) (n >= 0, 0 <= k <= n) =
 *          k(n-k) + 1.
 * https://oeis.org/A077028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(r,k) with r = row index where n falls, k = n -
 *            r(r+1)/2, and T(r,k) = k*(r-k) + 1 (this implements the rascal
 *            triangle read by rows)
 * Range:     u8 n <= 541, u16 n <= 11475, u32 n <= 11475, u64 n <= 11475,
 *            u128 n <= 11475, big any n
 * Validated: standard, pass, 107 terms through n = 8730 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t row_index_from_n(idx_t n) {
    /* Find the largest r such that r(r+1)/2 <= n.
       Uses binary search with seq_t arithmetic to avoid overflow. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until it exceeds the desired range. */
    while (((seq_t)high * (high + 1)) / 2 <= n) {
        if (high > (UINT64_MAX >> 1)) {
            /* Prevent infinite loop when shift would overflow. */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t m_val = ((seq_t)mid * (mid + 1)) / 2;
        if (m_val <= n)
            low = mid;
        else
            high = mid;
    }
    return low; /* row index r */
}

seq_t a(idx_t n) {
    /* Determine row r and column k for the n‑th element (0‑based). */
    idx_t r = row_index_from_n(n);

    /* Starting index of row r. */
    seq_t row_start_seq = ((seq_t)r * (r + 1)) / 2;
    idx_t row_start = (idx_t)row_start_seq; /* fits in idx_t by construction */

    idx_t k = n - row_start;                 /* column within the row */

    /* Compute the triangle entry: T(r,k) = k*(r-k) + 1. */
    seq_t term = ((seq_t)k * (seq_t)(r - k)) + 1;
    return term;
}
