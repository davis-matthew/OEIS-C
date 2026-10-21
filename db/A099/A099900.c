/*
 * A099900  XOR difference triangle, read by rows, of A099901 (in leftmost
 *          column) such that the main diagonal equals A099901 shift left and
 *          divided by 2.
 * https://oeis.org/A099900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099900 – XOR difference triangle read by rows.
    The triangle T(n,k) is defined for n,k≥0 by
           T(0,0)=1,
           T(n+1,0)=2·T(n,n),
           T(n,k)=T(n,k-1) XOR T(n‑1,k-1)   (k>0).
    The sequence a(n) is obtained by reading the
    rows of this triangle from top to bottom.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;
    unsigned __int128 sum_before = (unsigned __int128)r * (r + 1) / 2;
    idx_t col = n - (idx_t)sum_before;

    /* Special case: first row */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Generate rows up to r */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = (seq_t)1;          /* row 0 */

    for (idx_t i = 1; i <= r; ++i) {
        seq_t *cur = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        /* first element of the row */
        cur[0] = prev[i - 1] * (seq_t)2;

        /* remaining elements via XOR recurrence */
        for (idx_t j = 1; j <= i; ++j) {
            cur[j] = cur[j - 1] ^ prev[j - 1];
        }

        if (i == r) {
            seq_t result = cur[col];
            free(cur);
            free(prev);
            return result;
        }

        free(prev);
        prev = cur;
    }

    /* Should never reach here */
    free(prev);
    return (seq_t)0;
}
