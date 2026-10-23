/*
 * A185871  (Even,even)-polka dot array in the natural number array A000027,
 *          by antidiagonals.
 * https://oeis.org/A185871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (even,even)-polka dot array element indexed by
 *            antidiagonals of the triangular array T(n,k) = 2*n +
 *            (n+k-1)*(2*n+2*k-1), for n,k ≥ 1
 * Range:     u8 n <= 56, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Find the diagonal s such that total terms up to s-1 diagonals < n ≤ total up to s */
    idx_t low = 1;
    idx_t high = (n > 0) ? 2 * n : 1; /* s ≤ 2n for all n ≥ 1 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)(mid - 1) * mid / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low; /* diagonal index (sum of coordinates) */

    /* Compute previous cumulative count */
    idx_t prev_diag = s - 1;
    unsigned __int128 total_prev = (unsigned __int128)(prev_diag - 1) * prev_diag / 2;
    idx_t p = (idx_t)(n - total_prev); /* position within the diagonal (1‑based) */

    /* Determine coordinates: row n_row = p, column k = s - p */
    seq_t n_row = (seq_t)p;
    seq_t k_row = (seq_t)(s - p);

    /* Compute T(n_row, k_row) = 2*n_row + (n_row + k_row -1)*(2*n_row + 2*k_row -1) */
    seq_t term = 2 * n_row;
    seq_t term1 = n_row + k_row - 1;
    seq_t term2 = 2 * n_row + 2 * k_row - 1;
    term += term1 * term2;
    return term;
}
