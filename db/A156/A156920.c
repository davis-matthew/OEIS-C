/*
 * A156920  Triangle of the normalized A142963 and A156919 sequences.
 * https://oeis.org/A156920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Triangle T(n,m) with * T(0,0)=1, * T(n,0)=T(n,n)=1, * T(n,m) =
 *            (m+1)*T(n‑1,m) + (2*n-2*m+1)*T(n‑1,m-1) for 0<m<n * a(n) =
 *            linearised row‑wise form of T, i.e. a(n)=T(r,k) where * r is the
 *            unique integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2 and * k = n –
 *            r(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that offset(r) ≤ n < offset(r+1) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within row r */
    idx_t k = n - r * (r + 1) / 2;

    /* Special case: row 0 */
    if (r == 0)
        return (seq_t)1;

    /* Allocate arrays for two consecutive rows */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * r);       /* row r‑1 has r elements */
    if (!prev)
        return (seq_t)0;   /* allocation failure – return 0 as fallback */

    /* Initialise row 0 */
    prev[0] = (seq_t)1;                       /* T(0,0) = 1 */

    /* Build rows 1 … r, discarding older ones */
    for (idx_t cur = 1; cur <= r; ++cur) {
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (cur + 1));
        if (!curr) {
            free(prev);
            return (seq_t)0; /* allocation failure */
        }

        /* Boundary values */
        curr[0] = (seq_t)1;
        curr[cur] = (seq_t)1;

        /* Interior entries */
        for (idx_t m = 1; m < cur; ++m) {
            seq_t term1 = (seq_t)(m + 1) * prev[m];
            seq_t term2 = (seq_t)(2 * cur - 2 * m + 1) * prev[m - 1];
            curr[m] = term1 + term2;
        }

        /* Prepare for next iteration */
        free(prev);
        prev = curr;
    }

    /* Now 'prev' holds row r. Return the k-th value. */
    seq_t result = prev[k];
    free(prev);
    return result;
}
