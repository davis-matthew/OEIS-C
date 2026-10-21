/*
 * A225887  a(n) = A212205(2*n + 1).
 * https://oeis.org/A225887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A212205(2*n + 1) Computed as the diagonal T(n,n) of the
 *            lower‑triangular array T(0,0)=1, T(n,0)=1,
 *            T(n,k)=T(n,k-1)+T(n-1,k)+T(n-1,k-1) for 1 ≤ k ≤ n−1,
 *            T(n,n)=3*T(n-1,n-1)+T(n,n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* cache for previous and current rows */
    static seq_t *prev = NULL;
    static seq_t *curr = NULL;
    static size_t scr_prev_cap = 0;
    static size_t scr_curr_cap = 0;

    if (n == 0) {
        return (seq_t)1;
    }

    /* make sure we have space for rows up to length n+1 */
    ensure_capacity((size_t)(n + 1), &scr_prev_cap, (void **)&prev, sizeof(seq_t));
    ensure_capacity((size_t)(n + 1), &scr_curr_cap, (void **)&curr, sizeof(seq_t));

    /* base row: n = 0 */
    prev[0] = (seq_t)1;

    /* iterate rows 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        /* first element of the new row */
        curr[0] = (seq_t)1;

        /* fill the inner part of the row */
        for (idx_t k = 1; k < i; ++k) {
            curr[k] = curr[k - 1] + prev[k] + prev[k - 1];
        }

        /* last element of the row */
        curr[i] = (seq_t)3 * prev[i - 1] + curr[i - 1];

        /* make curr the new prev for the next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* after the loop, prev holds the last computed row */
    return prev[n];
}
