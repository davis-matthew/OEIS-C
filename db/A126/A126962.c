/*
 * A126962  Define an array by d(m, 0) = 1, d(m, 1) = m; d(m, k) = (m - k + 1)
 *          d(m+1, k-1) - (k-1) (m+1) d(m+2, k-2). Sequence gives d(1,n).
 * https://oeis.org/A126962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   d(m,0)=1, d(m,1)=m;
 *            d(m,k)=(m-k+1)d(m+1,k-1)-(k-1)(m+1)d(m+2,k-2). Then a(n)=d(1,n)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                 /* d(1,0) = 1 */

    seq_t S = (seq_t)n + (seq_t)1;                /* S = m + k = 1 + n  */

    /* d(m,k) at k = 0,1 */
    seq_t prevprev = (seq_t)1;                    /* d(S,0)  */
    seq_t prev     = S - (seq_t)1;                /* d(S-1,1)=S-1 */

    /* iterate k = 2 .. S-1  (final k = n  ->  m = 1) */
    for (idx_t k = 2; k < S; ++k)
    {
        seq_t term1 = (S - (seq_t)2*k + (seq_t)1) * prev;
        seq_t term2 = (seq_t)(k - 1) * (S - (seq_t)k + (seq_t)1) * prevprev;
        seq_t curr  = term1 - term2;

        prevprev = prev;
        prev     = curr;
    }

    return prev;   /* d(1,n) */
}
