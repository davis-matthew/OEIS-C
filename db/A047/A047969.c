/*
 * A047969  Square array of nexus numbers a(n,k) = (n+1)^(k+1) - n^(k+1) (n >=
 *          0, k >= 0) read by upwards antidiagonals.
 * https://oeis.org/A047969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n) = (row+1)^(k+1) - row^(k+1), where each term comes from the
 *            array a(row,k) = (row+1)^(k+1) - row^(k+1) read by upwards
 *            antidiagonals: for sum s = row + k, order is (s,0), (s-1,1), …,
 *            (0,s)
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 162, u64 n <= 422,
 *            u128 n <= 1112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the diagonal s such that the first two indices of the
       next diagonal exceed n. Each diagonal s has length s+1. */
    idx_t s = 0;
    idx_t cumulative = 0;          /* number of terms before diagonal s */
    while (1) {
        idx_t diag_len = s + 1;
        if (n < cumulative + diag_len) break;
        cumulative += diag_len;
        ++s;
    }

    /* Offset of n inside diagonal s.  The diagonal is traversed with
       n decreasing from s to 0. */
    idx_t j = n - cumulative;      /* 0 <= j <= s */
    idx_t row = s - j;             /* row index of a(row, j) */
    idx_t k   = j;                 /* column index of a(row, k) */

    idx_t exp = k + 1;             /* exponent (k+1) */
    seq_t base_plus  = (seq_t)(row + 1);
    seq_t base       = (seq_t)row;

    /* Compute (row+1)^(k+1) and row^(k+1) by repeated multiplication. */
    seq_t term1 = 1;
    for (idx_t i = 0; i < exp; ++i) term1 *= base_plus;

    seq_t term2 = 1;
    for (idx_t i = 0; i < exp; ++i) term2 *= base;

    return term1 - term2;
}
