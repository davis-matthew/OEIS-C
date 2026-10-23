/*
 * A185908  Array: T(n,k) = n-1 + min{n,k}, by antidiagonals.
 * https://oeis.org/A185908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by unfolding the infinite array * T(n,k) = n-1
 *            + min{n,k} (n,k ≥ 1) along antidiagonals. * Antidiagonals are
 *            indexed by s = n+k, and for a given s the * elements are taken
 *            in increasing k (i.e. n from 1 to s-1). * * For a given term
 *            index n ≥ 1: * 1. Find the smallest integer s s...
 * Range:     u8 n <= 120, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle n == 0 as undefined (not part of OEIS offset) */
    if (n == 0) return 0;

    /* Convert n to seq_t for safe arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Find s: smallest integer with n ≤ s*(s-1)/2 */
    idx_t low = 1, high = 2;
    /* Expand high until it satisfies the inequality */
    while ((seq_t)(high - 1) * (seq_t)high / (seq_t)2 < n_seq) high <<= 1;

    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t mid_total = (seq_t)(mid - 1) * (seq_t)mid / (seq_t)2;
        if (mid_total >= n_seq)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                  /* s ≥ 2 for all n ≥ 1 */

    /* Compute offset within the s‑th antidiagonal */
    seq_t prev_total = (seq_t)(s - 2) * (seq_t)(s - 1) / (seq_t)2;
    seq_t offset_seq = n_seq - prev_total - (seq_t)1; /* 0‑based */
    idx_t offset = (idx_t)offset_seq;                 /* fits in idx_t */

    /* Coordinates in the array */
    idx_t row = offset + (idx_t)1;   /* n */
    idx_t col = s - row;             /* k */

    /* Compute T(row, col) = row-1 + min(row, col) */
    idx_t min_rc = (row < col) ? row : col;
    seq_t result = (seq_t)(row - (idx_t)1) + (seq_t)min_rc;
    return result;
}
