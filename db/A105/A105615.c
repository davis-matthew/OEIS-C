/*
 * A105615  Triangular matrix T, read by rows, that satisfies:
 *          SHIFT_LEFT(column 0 of T^((2*p-1)/2)) = (2*p-1)*(column p of T),
 *          or [T^((2*p-1)/2)](m,0) = (2*p-1)*T(p+m,p+1) for all m>=1 and
 *          p>=0.
 * https://oeis.org/A105615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   mul128, row_of, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A105615: Triangle read by rows.
 * The triangle T satisfies
 *   T(n,n)   = 1                                  for n≥0
 *   T(n,0)   = A000698(n+1)                        (double factorial column)
 *   T(n,k)   = 2*(k+1)*T(n,k+1) + Σ_{j=1}^{n-k-1} T(j,0)*T(n,j+k+1)
 *        for n>k≥0
 * The sequence a(n) is obtained by reading T row by row.  For a given
 * index n, let r=row_of(n) (the unique row with r(r+1)/2≤n<(r+1)(r+2)/2)
 * and c=n−r(r+1)/2.  Then a(n)=T(r,c).  The recurrence for T is
 * implemented directly; no hard‑coded table is used.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/row_of.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul128.h"

seq_t a(idx_t n)
{
    /* static triangle storage, built lazily */
    static seq_t *T_data        = NULL;   /* flat array of all elements   */
    static idx_t T_computed_rows = 0;     /* rows 0 … T_computed_rows-1 already computed */
    static idx_t T_capacity      = 0;     /* allocated number of elements  */

    /* locate the row that contains index n */
    idx_t row = row_of(n);
    if (row >= T_computed_rows)
    {
        /* need to extend the triangle up to 'row' */
        idx_t needed_rows  = row + 1;
        idx_t needed_size  = needed_rows * (needed_rows + 1) / 2;

        if (needed_size > T_capacity)
        {
            /* reallocate, preserving previous content */
            seq_t *new_data = seq_realloc(T_data, needed_size);
            T_data   = new_data;
            T_capacity = needed_size;
        }

        /* compute all rows from T_computed_rows up to 'row' */
        for (idx_t r = T_computed_rows; r <= row; ++r)
        {
            idx_t start = r * (r + 1) / 2;             /* first element of row r */
            T_data[start + r] = (seq_t)1;              /* diagonal entry T(r,r)=1 */

            if (r > 0)
            {
                for (int k = (int)r - 1; k >= 0; --k)
                {
                    seq_t sum = 0;
                    idx_t maxj = r - (idx_t)k - 1;     /* j ranges 1..maxj */

                    for (idx_t j = 1; j <= maxj; ++j)
                    {
                        idx_t start_j = j * (j + 1) / 2;                /* row j start */
                        seq_t prod = mul128(T_data[start_j],           /* T(j,0)   */
                                            T_data[start + j + k + 1]);   /* T(r,j+k+1) */
                        sum += prod;
                    }

                    seq_t term = mul128(T_data[start + k + 1],
                                        (seq_t)(2 * (k + 1)));
                    T_data[start + k] = term + sum;
                }
            }
        }

        T_computed_rows = row + 1;
    }

    /* compute column index c within the row */
    idx_t col = n - row * (row + 1) / 2;
    return T_data[row * (row + 1) / 2 + col];
}
