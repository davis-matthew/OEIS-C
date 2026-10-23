/*
 * A173004  Square array A(n,k) read by antidiagonals: T(n,k) = A(n-k+1,k)
 *          where A(n,0)=0, A(n,1)=1, and A(n,k) = n*A(n,k-1) + k*A(n,k-2).
 * https://oeis.org/A173004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition for the triangle T(n,k) = A(n-k+1,k):
 *   A(n,0) = 0,
 *   A(n,1) = 1,
 *   A(n,k) = n*A(n,k-1) + k*A(n,k-2)  for k ≥ 2.
 *
 * Sequence a(n) is obtained by reading T row by row:
 *   a(n) = T(r,c) where r is the row satisfying
 *          (r+1)(r+2)/2 > n  and  prev = r(r+1)/2,
 *          c = n - prev.
 *   Hence  a(n) = A(r-c+1, c).
 */

seq_t a(idx_t n) {
    /* Find row r and column c corresponding to index n. */
    idx_t r = 0;           /* current row index */
    idx_t cum = 1;         /* cumulative number of entries up to row r */

    while (cum <= n) {
        r++;
        cum += (r + 1);    /* add the length of the new row (r+1) */
    }

    idx_t prev_cum = cum - (r + 1);   /* entries before current row */
    idx_t c = n - prev_cum;           /* column within the row */

    /* Compute A(x, k) with x = r - c + 1, k = c. */
    idx_t k = c;
    seq_t x = (seq_t)(r - c + 1);

    if (k == 0) return 0;
    if (k == 1) return 1;

    seq_t a0 = 0;  /* A(x,0) */
    seq_t a1 = 1;  /* A(x,1) */
    seq_t ai;

    for (idx_t j = 2; j <= k; ++j) {
        ai = x * a1 + (seq_t)j * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
