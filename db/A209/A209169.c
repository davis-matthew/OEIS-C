/*
 * A209169  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209168; see the Formula section.
 * https://oeis.org/A209169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle recurrence:  T(1,0)=1,  T(2,0)=2,  T(2,1)=3.
   For n>2,  T(n,k)=T(n-1,k)+2*T(n-1,k-1)+T(n-2,k)+T(n-2,k-2)
   (0 ≤ k < n).  a(n) is the n‑th element of the flattened triangle
   row‑wise, starting with a(1)=T(1,0). */
#include <stdlib.h>

/* forward declaration of the types supplied by the build system */
extern seq_t a(idx_t n);   /* declaration required by the build */

/* implementation of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n == 0, but return 0 */

    /* static cache of rows */
    static seq_t **rows = NULL;          /* rows[0] = row 1, rows[1] = row 2, ... */
    static idx_t rows_cap = 0;           /* number of rows stored so far */

    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;   /* r grows at most to about sqrt(2*n) */

    /* generate rows up to r */
    while (rows_cap < r)
    {
        idx_t cur = rows_cap + 1;   /* 1‑based row number to compute */

        /* allocate space for the new row */
        seq_t *row_mem = (seq_t *)malloc(cur * sizeof(seq_t));
        if (!row_mem)
            abort();   /* out of memory */

        if (cur == 1) {
            row_mem[0] = (seq_t)1;        /* T(1,0) */
        } else if (cur == 2) {
            row_mem[0] = (seq_t)2;        /* T(2,0) */
            row_mem[1] = (seq_t)3;        /* T(2,1) */
        } else {
            /* previous two rows */
            seq_t *prev1 = rows[cur - 2]; /* row n-1  */
            seq_t *prev2 = rows[cur - 3]; /* row n-2  */

            for (idx_t j = 0; j < cur; ++j)
            {
                seq_t t1 = (j < cur - 1) ? prev1[j] : (seq_t)0;                 /* T(n-1,k) */
                seq_t t2 = (j > 0)       ? prev1[j - 1] : (seq_t)0;             /* T(n-1,k-1) */
                seq_t t3 = (j < cur - 2) ? prev2[j] : (seq_t)0;                 /* T(n-2,k) */
                seq_t t4 = (j > 1)       ? prev2[j - 2] : (seq_t)0;             /* T(n-2,k-2) */

                row_mem[j] = t1 + (seq_t)2 * t2 + t3 + t4;
            }
        }

        /* grow the rows array and store the new row */
        seq_t **tmp = (seq_t **)realloc(rows, (rows_cap + 1) * sizeof(seq_t *));
        if (!tmp)
            abort();   /* out of memory */
        rows = tmp;
        rows[rows_cap] = row_mem;
        ++rows_cap;
    }

    /* determine column index within row r */
    idx_t prev_sum = (r - 1) * r / 2;   /* number of entries before row r */
    idx_t k = n - prev_sum - 1;         /* 0‑based column index */

    return rows[r - 1][k];
}
