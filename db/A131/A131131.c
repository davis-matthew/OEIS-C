/*
 * A131131  4*A007318 - 3*A097806.
 * https://oeis.org/A131131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4 * C(r, k) – 3 if k ≥ r–1 4 * C(r, k) otherwise where r
 *            is the largest integer with r·(r+1)/2 ≤ n (row number) and k = n
 *            – r·(r+1)/2 (column number)
 * Range:     u8 n <= 39, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* --- locate the row r and column k for index n ------------------ */
    idx_t r   = 0;   /* current row */
    idx_t sum = 0;   /* number of elements in all previous rows */
    /* Find largest r such that sum + r + 1 ≤ n */
    while (sum + r + 1 <= n) {
        r++;
        sum += r;
    }
    idx_t k = n - sum;   /* position inside the row r */

    /* --- compute binomial coefficient C(r,k) ------------------------ */
    seq_t binom = 1;
    idx_t kk = (k > r - k) ? (r - k) : k;   /* use symmetry */
    for (idx_t i = 1; i <= kk; ++i) {
        binom = binom * (r - kk + i);
        binom = binom / i;
    }

    /* --- final value ------------------------------------------------- */
    seq_t val = binom * 4;
    if (k == r || k == r - 1) {
        val -= 3;
    }
    return val;
}
