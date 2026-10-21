/*
 * A223513  Triangle T(n,k) represents the coefficients of (x^11*d/dx)^n,
 *          where n=1,2,3,...
 * https://oeis.org/A223513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = (10*(n-1)+k)*T(n-1,k) + T(n-1,k-1) * with T(1,1)=1. The
 *            sequence a(n) is the flattened array * of the triangle rows:
 *            a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *values = NULL;     /* flattened triangle values            */
    static size_t  values_cap = 0;   /* allocated capacity of values array  */
    static size_t  values_len = 0;   /* number of values stored             */

    /* array for the previous row (previously computed row) */
    static seq_t *prev_row = NULL;   /* array of size prev_len+1            */
    static size_t prev_cap  = 0;
    static idx_t  prev_len  = 0;     /* length of the previous row          */

    /* pattern: if we have not yet produced any terms,
       initialise the first row (T(1,1)=1). */
    if (values_len == 0)
    {
        /* allocate reciprocally for the first row */
        prev_cap = 2;                /* indices: 0 .. 1, 1 will hold T(1,1) */
        prev_row = (seq_t *)malloc(prev_cap * sizeof(seq_t));
        prev_row[0] = 0;
        prev_row[1] = 1;
        prev_len  = 1;

        /* ensure capacity for the values array */
        values_cap = 16;
        values     = (seq_t *)malloc(values_cap * sizeof(seq_t));
        values[0] = prev_row[1];
        values_len = 1;
    }

    /* generate rows until we have at least n values */
    while (values_len < n)
    {
        /* compute next row r = prev_len + 1 */
        idx_t r = prev_len + 1;

        /* allocate array for the new row (size r+1) */
        seq_t *curr_row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        curr_row[0] = 0;

        /* compute each entry of the new row */
        for (idx_t k = 1; k <= r; ++k)
        {
            seq_t term1 = 0;
            if (k <= prev_len)
            {
                /* (10*(n-1)+k) * T(n-1,k)  (n-1 = prev_len) */
                term1 = (seq_t)((10 * prev_len) + k) * prev_row[k];
            }
            seq_t term2 = (k > 1) ? prev_row[k - 1] : 0;
            curr_row[k] = term1 + term2;

            /* append to the flattened values array */
            if (values_len == values_cap)
            {
                values_cap = values_cap ? 2 * values_cap : 16;
                values = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
            }
            values[values_len++] = curr_row[k];
        }

        /* free the old row and promote the new row to prev_row */
        free(prev_row);
        prev_row = curr_row;
        prev_len = r;
    }

    return values[n - 1];
}
