/*
 * A121875  Triangular array read by rows: see Comments for definition.
 * https://oeis.org/A121875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangular array formed from the
 *            infinite sequences R(m,·) defined by: R(m,0)=R(m,1)=1;
 *            R(m,i)=m*R(m,i-1)+R(m,i-2). The array is read by columns: first
 *            column (index 0) contributes only the term R(1,0)=1 (all
 *            R(m,0)=1 are equal, so only one is taken). For each subs...
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)          /* first term only */
        return (seq_t)1;

    /* Find smallest column k such that k(k+1)/2 >= n */
    idx_t k = 1;
    idx_t cum = 1;               /* cumulative count T(1) = 1*2/2 = 1 */
    while (cum < n) {
        ++k;
        cum += k;                /* cum = k(k+1)/2 now */
    }

    /* Number of terms before this column */
    idx_t prev = cum - k;        /* T(k-1) = k(k-1)/2 */

    /* Position within the current column (0‑based) */
    idx_t pos = n - prev - 1;
    idx_t m   = pos + 1;         /* row index, 1 ≤ m ≤ k */

    /* Compute R(m,k) by iterating the recurrence */
    seq_t r0 = 1;                /* R(m,0) */
    seq_t r1 = 1;                /* R(m,1) */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t r2 = (seq_t)m * r1 + r0;  /* R(m,i) = m*R(m,i-1)+R(m,i-2) */
        r0 = r1;
        r1 = r2;
    }
    return r1;                   /* r1 now holds R(m,k) */
}
