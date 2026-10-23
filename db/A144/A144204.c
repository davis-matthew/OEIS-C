/*
 * A144204  Array A(k,n) = (n+k-2)*(n-1) - 1 (k >= 1, n >= 1) read by
 *          antidiagonals.
 * https://oeis.org/A144204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = read A(k,n) = (n+k-2)*(n-1)-1 by antidiagonals of the
 *            two‑dimensional array A(k,n) for k,n >= 1. A(n) is the n‑th term
 *            (a(1) is the first term)
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal d (d ≥ 2) such that the cumulative
       number of terms up to that diagonal is at least n.
       The number of terms up to diagonal d is (d‑1)*d/2. */
    idx_t N = n;
    idx_t d = 1;
    while (((__int128)(d - 1) * d / 2) < N)
        ++d;                     /* d grows like √(2N) for N≥1 */

    /* Number of terms before diagonal d */
    __int128 cumPrev = (__int128)(d - 2) * (d - 1) / 2;

    /* Position inside the diagonal (1 … d‑1) */
    idx_t offset = N - (idx_t)cumPrev;

    /* For each diagonal we list the entries with
       decreasing column index k = d-1 … 1.
       offset = 1 corresponds to the largest k (k = d‑1). */
    idx_t k = d - offset;   /* column index (1 … d‑1) */
    idx_t row = offset;     /* row index n (1 … d‑1) */

    /* Compute A(k,row) = (row + k - 2)*(row - 1) - 1 */
    seq_t term = ((seq_t)k + (seq_t)row - 2) * ((seq_t)row - 1) - 1;
    return term;
}
