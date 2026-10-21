/*
 * A225466  Triangle read by rows, 3^k*S_3(n, k) where S_m(n, k) are the
 *          Stirling-Frobenius subset numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is read from the Sheffer triangle T(n,k) with recurrence
 *            T(0,0)=1, T(n,0)=2*T(n-1,0), and for k>=1 T(n,k) =
 *            (2+3*k)*T(n-1,k) + 3*T(n-1,k-1)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 58, u64 n <= 175,
 *            u128 n <= 503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle indices are extracted from the linear index n:
   row r satisfies n < (r+1)(r+2)/2.
   column k = n - r(r+1)/2.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find row r such that n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;

    /* base case */
    if (r == 0)
        return (k == 0) ? (seq_t)1 : (seq_t)0;

    /* arrays for the current and previous row, of length k+1 */
    size_t len = (size_t)(k + 1);
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return (seq_t)0;          /* memory allocation failure */
    }

    prev[0] = (seq_t)1;           /* T(0,0) */

    for (idx_t i = 1; i <= r; ++i) {
        /* T(i,0) */
        cur[0] = prev[0] * (seq_t)2;

        /* limit for the inner loop: j <= min(i,k) */
        idx_t limit = (i < k) ? i : k;
        for (idx_t j = 1; j <= limit; ++j) {
            cur[j] = prev[j] * (seq_t)(2 + 3 * j)
                   + (seq_t)3 * prev[j - 1];
        }

        /* zero remaining entries (if any) */
        for (idx_t j = limit + 1; j <= k; ++j)
            cur[j] = 0;

        /* swap pointers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(cur);
    return result;
}
