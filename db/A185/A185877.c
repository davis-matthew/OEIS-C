/*
 * A185877  Array T given by T(n,k) = k^2 +(2*n-3)*k -2*n +3, by
 *          antidiagonals.
 * https://oeis.org/A185877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) = k^2 + (2*n-3)*k - 2*n + 3 enumerated by
 *            antidiagonals
 * Range:     u8 n <= 44, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 >= n  (d <= n). */
    idx_t low = 1;
    idx_t high = n;   /* d cannot exceed n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t mid_seq = (seq_t)mid;
        seq_t tmid = mid_seq * (mid_seq + 1) / 2;
        if (tmid >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;
    seq_t d_seq = (seq_t)d;

    /* Number of terms before diagonal d */
    seq_t t_prev = d_seq * (d_seq - 1) / 2;

    /* Zero‑based offset within the diagonal */
    seq_t offset0 = (seq_t)n - t_prev - 1;

    /* Coordinates in the array */
    seq_t row = offset0 + 1;      /* n-index (row)   */
    seq_t col = d_seq - offset0;  /* k-index (col)   */

    /* Compute T(row,col) safely avoiding unsigned underflow */
    seq_t tmp = col * col;                /* k^2                 */
    tmp += (seq_t)(2 * row) * col;        /* + 2*n*k             */
    tmp -= 2 * row;                      /* - 2*n               */
    tmp += 3;                            /* + 3                  */
    seq_t result = tmp - 3 * col;         /* - 3*k               */

    return result;
}
