/*
 * A193735  Mirror of the triangle A193734.
 * https://oeis.org/A193735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 122, u64 n <= 439,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A193735 – mirror of triangle A193734.
    The sequence is obtained from the triangle T(n,k) defined by:
        T(0,0) = 1,
        T(1,0) = 2, T(1,1) = 1,
        T(n,k) = T(n-1,k-1) + 4*T(n-1,k)          (k ≥ 0, T(n-1,−1)=0, T(n-1,k>n-1)=0).
    For a given index n:
        r = ⌊ (√(8n+1)–1)/2 ⌋          – row containing n
        t = n – r(r+1)/2              – position within that row
        a(n) = T(r,t).                 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r that contains index n.
       Invariant: r*(r+1)/2 ≤ n < (r+1)(r+2)/2                            */
    idx_t r = 0;
    while (1)
    {
        idx_t r1 = r + 1;
        idx_t s = r1 * (r1 + 1) / 2;
        if (s > n) break;
        r = r1;
    }

    /* Static storage for already computed rows.  */
    static seq_t **rows = NULL;     /* array of pointers to rows */
    static idx_t rows_used = 0;     /* number of rows that exist */

    /* grow array of row pointers if necessary */
    if (rows_used <= r)
    {
        if (!rows)
            rows = (seq_t**)malloc((r + 1) * sizeof(seq_t*));
        else
            rows = (seq_t**)realloc(rows, (r + 1) * sizeof(seq_t*));

        /* compute the missing rows from rows_used up to r				 */
        for (idx_t i = rows_used; i <= r; ++i)
        {
            idx_t len = i + 1;
            seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
            rows[i] = row;

            if (i == 0)
            {
                row[0] = (seq_t)1;
            }
            else if (i == 1)
            {
                row[0] = (seq_t)2;
                row[1] = (seq_t)1;
            }
            else
            {
                seq_t *prev = rows[i - 1];
                for (idx_t k = 0; k <= i; ++k)
                {
                    seq_t val = 0;
                    if (k > 0)
                        val += prev[k - 1];
                    if (k <= i - 1)
                        val += ((seq_t)4) * prev[k];
                    row[k] = val;
                }
            }
        }
        rows_used = r + 1;
    }

    /* Compute position t within row r.  */
    idx_t offset = r * (r + 1) / 2;
    idx_t t = n - offset;

    return rows[r][t];
}
