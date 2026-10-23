/*
 * A013613  Triangle of coefficients in expansion of (1+6*x)^n.
 * https://oeis.org/A013613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = binomial(r,k) * 6^k, where r is the unique integer with
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n - r(r+1)/2
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 88, u64 n <= 318,
 *            u128 n <= 1165, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine row r such that Tri(r) ≤ n < Tri(r+1) */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        r++;
    r--;                       /* now r satisfies the inequality */

    idx_t start = (r * (r + 1)) / 2; /* first index of row r       */
    idx_t k_offset = n - start;      /* column within the row     */

    /* Compute binomial coefficient C(r,k_offset) */
    idx_t k = k_offset;          /* temporary copy for binomial computation */
    if (k > r)
        return 0;
    if (k > r - k)
        k = r - k;                /* use symmetry to minimise multiplications */

    seq_t bin = 1;
    for (idx_t i = 1; i <= k; ++i) {
        bin = (bin * (seq_t)(r - k + i)) / (seq_t)i;
    }

    /* Compute 6^k_offset */
    seq_t pow6 = 1;
    for (idx_t i = 0; i < k_offset; ++i) {
        pow6 = pow6 * (seq_t)6;
    }

    return pow6 * bin;
}
