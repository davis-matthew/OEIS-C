/*
 * A134235  Triangle read by rows: a(1) = 1; then n-th row = n terms of (n+2),
 *          (n+4), (n+6), ..., n.
 * https://oeis.org/A134235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows. * Each
 *            row r (starting at r=1) contains the numbers * r, r+2, r+4, … ,
 *            r+2(r-1) * but the row is cyclically shifted so that the first
 *            element shown * is r+2. The linear index n is 1‑based. * * For
 *            an index n, let r be the unique row such that...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that T(r-1) < n <= T(r) using binary search. */
    idx_t low = 1;
    idx_t high = n;          /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute the start index of row r: T(r-1) + 1. */
    idx_t sum_prev = (idx_t)(((__uint128_t)(r - 1) * r) / 2);
    idx_t start = sum_prev + 1;

    /* Position within the row (0‑based). */
    idx_t pos = n - start;

    /* Compute the value: r + 2 * ((pos + 1) mod r). */
    seq_t s_r  = (seq_t)r;
    seq_t s_pos_plus1 = (seq_t)(pos + 1);
    seq_t s_r_mod = s_r;                 /* same as r but in seq_t ------------------*/
    seq_t val = s_r + ((seq_t)2 * (s_pos_plus1 % s_r_mod));

    return val;
}
