/*
 * A036969  Triangle read by rows: T(n,k) = T(n-1,k-1) + k^2*T(n-1,k), 1 < k
 *          <= n, T(n,1) = 1.
 * https://oeis.org/A036969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl, changed
 * Formula:   a(n) = T(r,k) where r(r−1)/2 < n ≤ r(r+1)/2, k = n − r(r−1)/2,
 *            T(1,1)=1, T(n,1)=1 for n>1, T(n,k)=T(n−1,k−1)+k²·T(n−1,k) for
 *            1<k≤n (triangle read by rows)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 69, u64 n <= 158,
 *            u128 n <= 413, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache of the computed rows */
static seq_t **triangle = NULL;          /* triangle[r] points to array of length r+1 (index 0 unused) */
static idx_t   max_row   = 0;            /* largest row currently stored */
static idx_t   cumulative = 0;           /* total number of entries in rows 1..max_row */

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* generate rows until we have at least n entries */
    while (cumulative < n)
    {
        idx_t r = max_row + 1;                       /* new row index */
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
        if (!row) return (seq_t)0;                   /* allocation failure */

        row[0] = 0;                                  /* unused */

        /* base case r == 1 */
        if (r == 1)
        {
            row[1] = (seq_t)1;
        }
        else
        {
            row[1] = (seq_t)1;                       /* first element is 1 */
            row[r] = (seq_t)1;                       /* last element is 1 */

            /* compute interior elements */
            for (idx_t k = 2; k < r; ++k)
            {
                seq_t prev1 = triangle[max_row][k-1];   /* T(r−1,k−1) */
                seq_t prev2 = triangle[max_row][k];     /* T(r−1,k) */
                seq_t kk    = (seq_t)k * k;             /* k² */
                row[k] = prev1 + kk * prev2;            /* recurrence */
            }
        }

        /* extend the triangle array */
        triangle = (seq_t **)realloc(triangle, sizeof(seq_t *) * (r + 1));
        if (!triangle) return (seq_t)0;                /* allocation failure */
        triangle[0] = NULL;                            /* keep index 0 unused */
        triangle[r] = row;

        cumulative += r;
        max_row = r;
    }

    /* locate the desired entry */
    idx_t prev_cumulative = (max_row - 1) * max_row / 2;
    idx_t k = n - prev_cumulative;
    return triangle[max_row][k];
}
