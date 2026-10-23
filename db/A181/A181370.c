/*
 * A181370  Square array read by antidiagonals: T(m,n) is the number of
 *          L-convex polyominoes with m rows and n columns.
 * https://oeis.org/A181370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A181370 is read by antidiagonals of the square array with
 *            entries T(m,n) = number of L‑convex polyominoes with m rows and
 *            n columns. T(m,n) is given by T(m,n) = 1 (m,n)=(1,1) T(m,n) =
 *            Σ_{k=0..min(m-1,n-1)} 2^k * (k^2 + (m+n-2)k + mn-1) * C(m+n-2,
 *            2k) * C(m+n-2-2k, m-1-k) / ((m+n-2)(2k+1)) otherwi...
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: binomial coefficient C(n,k), n,k up to about 300. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)               return 1;          /* C(n,k)=0 for k>n but never needed   */
    if (k == 0 || k == n)    return 1;
    if (k > n - k)           k = n - k;        /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);   /* multiply by (n-k+i)   */
        res /= (seq_t)i;             /* divide by i – exact */
    }
    return res;
}

/* Compute T(m,n) for m,n >= 1.  */
static seq_t T_convex(idx_t m, idx_t n)
{
    if (m == 1 && n == 1) return 1;          /* special case */
    idx_t maxk = (m < n) ? (m - 1) : (n - 1);
    seq_t sum   = 0;
    seq_t two   = 1;                          /* 2^k, starts at k=0 */
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t poly = (seq_t)k * (seq_t)k              /* k^2 */
                   + (seq_t)(m + n - 2) * (seq_t)k     /* (m+n-2)k */
                   + (seq_t)m * (seq_t)n - (seq_t)1;   /* mn-1 */

        seq_t b1 = binom_idx(m + n - 2, 2 * k);
        seq_t b2 = binom_idx(m + n - 2 - 2 * k, m - 1 - k);

        seq_t term = two * poly;
        term *= b1;
        term *= b2;
        term /= (seq_t)(m + n - 2);          /* divide by (m+n-2)   */
        term /= (seq_t)(2 * k + 1);          /* divide by (2k+1)    */
        sum += term;

        two *= (seq_t)2;                     /* advance 2^{k+1}     */
    }
    return sum;
}

/* Main entry: a(n) for n >= 1.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    /* Find the diagonal d so that d*(d-1)/2 >= n   (d starts at 2). */
    idx_t d = 2;
    while ((d - 1) * d / 2 < n) ++d;
    idx_t prev = (d - 2) * (d - 1) / 2;   /* count before diagonal d */
    idx_t offset = n - prev - 1;          /* 0-based offset in this diagonal */
    idx_t row = offset + 1;               /* m   */
    idx_t col = d - offset - 1;           /* n   */
    return T_convex(row, col);
}
