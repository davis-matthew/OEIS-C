/*
 * A156286  Triangle T(n, k) = (1/k^n)*Product_{j=1..n} ( (k-1)*(k+1)^j + 1 ),
 *          read by rows.
 * https://oeis.org/A156286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = (∏_{j=1..r} ((k-1)*(k+1)^j + 1)) /
 *            k^r. The indices r (row) and k (column) are obtained from the
 *            linear index n by r = smallest integer with r*(r+1)/2 ≥ n, k = n
 *            − r*(r−1)/2. The value is computed as the product of (term/k)
 *            for j = 1..r, exploiting that each term is d...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Determine row r and column k from linear index n.  r is the
       smallest integer satisfying r*(r+1)/2 ≥ n.  */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        r++;
    }
    idx_t prev_tri = (r - 1) * r / 2;   /* triangular number T(r-1) */
    idx_t k = n - prev_tri;             /* 1‑based column index */

    seq_t k_seq = (seq_t)k;
    seq_t base_factor = (seq_t)(k - 1);          /* (k-1) */
    seq_t k_plus1 = k_seq + 1;                  /* (k+1) */
    seq_t pow_j = k_plus1;                      /* (k+1)^j, starts with j=1 */

    /* Compute the product of (term/k) for j = 1..r.  Each term is
       integer multiple of k, so division is exact.  */
    seq_t result = 1;
    for (idx_t j = 1; j <= r; ++j) {
        seq_t term = base_factor * pow_j + 1;     /* ((k-1)*(k+1)^j + 1) */
        seq_t term_div = term / k_seq;           /* exact division by k */
        result *= term_div;
        if (j < r) {
            pow_j *= k_plus1;                     /* advance to (k+1)^{j+1} */
        }
    }
    return result;
}
