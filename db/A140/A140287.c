/*
 * A140287  Table T(n,k) = -2*T(n-1,k)+T(n-1,k+1) = T(n,k-4), 0<=n.
 * https://oeis.org/A140287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   A140287 concatenates the rows of the table T(n,k) defined by
 *            T(0,1)=0, T(0,2)=0, T(0,3)=1, T(0,4)=-1 and for n>0 T(n,k)=
 *            -2*T(n‑1,k)+T(n‑1,k+1) with indices taken modulo 4. The sequence
 *            a(n) is the row‑major listing of these rows
 * Range:     i8 n <= 20, i16 n <= 43, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* number of columns in each row */
    static const idx_t COLS = 4;

    /* cache of all computed rows */
    static seq_t *cache = NULL;          /* flattened [row][col] array   */
    static idx_t cached_rows = 0;        /* number of rows currently cached */
    static idx_t cap_rows = 0;           /* allocated capacity in rows    */

    /* determine which row and column this n refers to */
    idx_t row = n / COLS;
    idx_t col = n % COLS;

    /* grow the cache until it contains 'row' */
    while (cached_rows <= row)
    {
        /* initialisation */
        if (cached_rows == 0) {
            /* ensure at least one row of capacity */
            if (cap_rows == 0) cap_rows = 1;
            cache = (seq_t *)malloc(sizeof(seq_t) * cap_rows * COLS);
            /* base row */
            cache[0] = (seq_t)0;          /* T(0,1) */
            cache[1] = (seq_t)0;          /* T(0,2) */
            cache[2] = (seq_t)1;          /* T(0,3) */
            cache[3] = (seq_t)-1;         /* T(0,4) */
            cached_rows = 1;
            continue;
        }

        /* if we would exceed current allocated rows, grow the buffer */
        if (cached_rows >= cap_rows) {
            idx_t new_cap = cap_rows == 0 ? 1 : cap_rows * 2;
            while (new_cap <= cached_rows * 2) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap * COLS);
            cache = tmp;
            cap_rows = new_cap;
        }

        /* compute the next row using the recurrence */
        seq_t *prev = cache + (cached_rows - 1) * COLS;
        seq_t *curr = cache + cached_rows * COLS;

        curr[0] = -2 * prev[0] + prev[1];
        curr[1] = -2 * prev[1] + prev[2];
        curr[2] = -2 * prev[2] + prev[3];
        curr[3] = -2 * prev[3] + prev[0];

        cached_rows++;
    }

    /* return the requested element */
    return cache[row * COLS + col];
}
