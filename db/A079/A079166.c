/*
 * A079166  Square array read by antidiagonals of
 *          T(n,k)=(4k-2)*T(n,k-1)+T(n,k-2) with T(n,0)=1 and T(n,1)=n.
 * https://oeis.org/A079166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals of the triangle T(n,k) defined by
 *            * T(n,0)=1, T(n,1)=n, T(n,k) = (4k-2)*T(n,k-1)+T(n,k-2) (k≥2). *
 *            The index n is converted to the pair (row=row_index,
 *            col=col_index) by: * Find the smallest s such that s*(s+1)/2 >
 *            n. * Let total = s*(s+1)/2 - (s+1). * row = n - tota...
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal s and offset total = s*(s+1)/2 .  */
    idx_t s = 0;
    idx_t total = 0;               /* total terms before diagonal s   */
    while (1) {
        /* next_total = total + (s+1) */
        idx_t next_total = total + (s + 1ull);
        if (n < next_total)
            break;
        total = next_total;
        s++;
    }

    idx_t row = n - total;         /* row index i      */
    idx_t col = s - row;           /* column index j   */

    /* Compute T(row, col) by forward recurrence in k.  */
    seq_t prev_prev = 1;           /* T(row,0) */
    if (col == 0)
        return prev_prev;

    seq_t prev = (seq_t)row;       /* T(row,1) */
    if (col == 1)
        return prev;

    for (idx_t k = 2; k <= col; ++k) {
        seq_t cur = ((seq_t)(4 * k - 2)) * prev + prev_prev;
        prev_prev = prev;
        prev = cur;
    }
    return prev;
}
