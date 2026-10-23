/*
 * A153653  Triangle T(n, k, j) = T(n-1, k, j) + T(n-1, k-1, j) + (2*j +
 *          1)*prime(j)*T(n-2, k-1, j) with T(2, k, j) = prime(j) and j = 8,
 *          read by rows.
 * https://oeis.org/A153653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   T(1,1)=2; T(2,1)=T(2,2)=19; * T(3,1)=T(3,3)=2,
 *            T(3,2)=2*prime(8)^2-4=718. * For n≥4 and 1<k<n : *
 *            T(n,k)=T(n-1,k)+T(n-1,k-1)+323·T(n-2,k-1) (prime(8)=19,
 *            2*j+1=17). * The sequence a(n) is obtained by reading the
 *            triangle row‑by‑row * (row 1 has 1 element, row 2 has 2
 *            elements, etc.). * a(n)=T(r,c...
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 40, u64 n <= 143, u128 n <= 510,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of rows */
    static seq_t **rows   = NULL;   /* rows[1] .. rows[maxRow] */
    static idx_t  maxRow  = 0;      /* highest computed row */

    /* locate the row that contains the nth entry */
    idx_t r  = 1;
    idx_t cum = 1;                  /* cumulative count up to row r */
    while (cum < n)
    {
        ++r;
        cum += r;
    }

    /* extend cache up to row r */
    if (r > maxRow)
    {
        seq_t **tmp = (seq_t**)realloc(rows, (r + 1) * sizeof(seq_t*));
        rows = tmp;

        for (idx_t i = maxRow + 1; i <= r; ++i)
        {
            rows[i] = (seq_t*)malloc(i * sizeof(seq_t));
            if (i == 1)
            {
                rows[1][0] = (seq_t)2;
            }
            else if (i == 2)
            {
                rows[2][0] = rows[2][1] = (seq_t)19;
            }
            else if (i == 3)
            {
                rows[3][0] = (seq_t)2;
                rows[3][2] = (seq_t)2;
                rows[3][1] = (seq_t)718;             /* 2*19^2-4 */
            }
            else
            {
                rows[i][0]     = (seq_t)2;
                rows[i][i - 1] = (seq_t)2;
                for (idx_t k = 1; k < i - 1; ++k)
                {
                    seq_t left   = rows[i - 1][k];        /* T(n-1,k) */
                    seq_t up     = rows[i - 1][k - 1];    /* T(n-1,k-1) */
                    seq_t diag   = rows[i - 2][k - 1];    /* T(n-2,k-1) */
                    rows[i][k]   = left + up + (seq_t)323 * diag;
                }
            }
        }
        maxRow = r;
    }

    idx_t prevCum = (r - 1) * r / 2;     /* number of entries before row r */
    idx_t offset  = n - prevCum;         /* 1‑based position within row r */

    return rows[r][offset - 1];
}
