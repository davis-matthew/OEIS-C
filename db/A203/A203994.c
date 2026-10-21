/*
 * A203994  Symmetric matrix based on f(i,j) = (i+j)*min{i,j}, by
 *          antidiagonals.
 * https://oeis.org/A203994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = 1 - |i - j|, where (i,j) is the n-th element in the *
 *            antidiagonal enumeration of the infinite matrix M with * M(i,j)
 *            = 1 - |i-j| (starting from (1,1))
 * Range:     i8 n <= 5050, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Off‑by‑one: the sequence is indexed from 1. */
    if (n == 0)
        return (seq_t)0;

    /* Find the diagonal k such that the cumulative count up to k-1
     * is < n and up to k is >= n.
     * The number of elements in diagonal k is k-1, and the
     * cumulative number up to diagonal k is (k-1)k/2. */
    idx_t low = 2;
    idx_t high = n + 1;               /* k cannot exceed n+1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use __int128 for safe multiplication */
        __int128 cum = (__int128)(mid - 1) * mid / 2;
        if ((idx_t)cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                    /* diagonal index (i+j) */

    /* Elements before this diagonal */
    __int128 prev = (__int128)(k - 1) * (k - 2) / 2;
    idx_t before = (idx_t)prev;

    /* Position inside diagonal k (0‑based) */
    idx_t offset = n - before - 1;           /* 0 <= offset <= k-2 */

    /* Compute |2*offset - k + 2| using signed intermediate to avoid overflow */
    long long diff_sign = (long long)offset * 2 - (long long)k + 2;
    if (diff_sign < 0)
        diff_sign = -diff_sign;

    /* Value = 1 - diff */
    return (seq_t)1 - (seq_t)diff_sign;
}
