/*
 * A223527  Triangle S(n,k) by rows: coefficients of
 *          4^((n-1)/2)*(x^(1/4)*d/dx)^n when n=1,3,5,...
 * https://oeis.org/A223527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For the triangle entry at row m (m ≥ 1) and column k (0 ≤ k <
 *            m), a(m,k) = C(m–1, k) × 4^k × ∏_{r=k+1}^{m–1} (4r+1), where C
 *            denotes the binomial coefficient. The sequence is the flattened
 *            triangle: row 1 has 1 entry, row 2 has 2, etc. The index n in
 *            the sequence satisfies (m–1)m/2 < n ≤ m(m+1)/2...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find row m */
    idx_t m = 1;
    while ((m * (m + 1) / 2) < n)
        ++m;
    /* Compute column k */
    idx_t prev = (m - 1) * m / 2;
    idx_t k = n - prev - 1;
    /* Binomial coefficient C(m-1, k) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        binom = binom * (seq_t)(m - 1 - (k - i)) / (seq_t)i;
    }
    /* Power 4^k */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow4 *= (seq_t)4;
    /* Product ∏_{r=k+1}^{m-1} (4r+1) */
    seq_t prod = 1;
    for (idx_t r = k + 1; r <= m - 1; ++r)
        prod *= (seq_t)(4 * r + 1);
    return binom * prod * pow4;
}
