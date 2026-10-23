/*
 * A130879  An antidiagonal triangular sequence based on sums of fractal
 *          self-similar level count totals of the sort: Sum_{n=0..m} k^(2^n).
 * https://oeis.org/A130879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 20, u64 n <= 27, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = antidiagonal transform of the triangular array
 *  a(m,k) = Σ_{i=0..m} k^(2^i).
 *  The sequence is indexed from n = 1, giving the terms
 *  2, 3, 6, 4, 12, 22, 5, 20, 93, 278, … as in A130879.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal (sum index) d such that
     * sum_{t=0}^{d} (t+1) >= n.
     * sum = 1 + 2 + … + (d+1) = (d+1)(d+2)/2. */
    idx_t t   = 0;   /* number of terms added so far == (t choose 1) */
    idx_t sum = 0;   /* prefix sum of diagonal lengths */

    while (sum < n) {
        ++t;          /* next diagonal length (t) */
        sum += t;     /* add length of current diagonal to prefix */
    }
    /* After loop: sum == prefix(d) where d = t-1 */
    idx_t d            = t - 1;          /* diagonal index (starting at 0)   */
    idx_t prefix_prev  = sum - t;        /* sum of all earlier diagonals     */
    idx_t pos          = n - prefix_prev - 1; /* position inside diagonal (0‑based) */
    idx_t m            = pos;                /* row index                       */
    idx_t k            = 2 + (d - pos);      /* column value k (k ≥ 2)           */

    /* Compute a(m,k) = Σ_{i=0..m} k^(2^i) */
    seq_t total = (seq_t)0;
    seq_t cur   = (seq_t)k; /* k^(2^0) */

    for (idx_t i = 0; i <= m; ++i) {
        total += cur;          /* add current power */
        if (i < m) {           /* prepare next power: square the previous one */
            cur = cur * cur;
        }
    }
    return total;
}
