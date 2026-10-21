/*
 * A204202  Triangle based on (0,2/3,1) averaging array.
 * https://oeis.org/A204202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence defining the triangle T(n,k):
   T(n,k) = T(n-1,k) + 3*T(n-1,k-1) - 2*T(n-2,k-1) - 2*T(n-2,k-2),
   with T(1,1)=2, T(2,1)=2, T(2,2)=5, and T(n,k)=0 for k<1 or k>n.
   The sequence a(n) is the flattening of the triangle row‑by‑row:
   a(n)=T(i,j) where i is the smallest integer with i(i+1)/2 ≥ n
         and j = n - (i-1)i/2.  */

/* standard C compatibility */
#include <stdlib.h>

/* The following types are defined by the build system:
   seq_t  – unsigned integer of 8,16,32,64,128 bits, or an arbitrary‑precision class
   idx_t  – unsigned integer, typically uint64_t                                           */

static idx_t   max_row   = 0;          /* highest row computed */
static seq_t **rows      = NULL;       /* rows[1..max_row] are pointers to row arrays */
static size_t  rows_cap  = 0;          /* allocated capacity of rows array */

/* Ensure rows[1..need] are computed and stored. */
static void
ensure_rows(idx_t need)
{
    if (need <= max_row)
        return;

    /* grow rows array if necessary */
    if (need > rows_cap) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 8;
        while (need > new_cap)
            new_cap *= 2;
        seq_t **tmp = (seq_t **)realloc(rows,
                                        (new_cap + 1) * sizeof(seq_t *));
        if (!tmp)
            abort();                    /* out of memory */
        rows      = tmp;
        rows_cap  = new_cap;
    }

    /* compute new rows up to 'need' */
    while (max_row < need) {
        ++max_row;
        /* allocate an array of max_row+1 elements (index 0 unused) */
#ifdef __cplusplus
        seq_t *row = new seq_t[max_row + 1];
#else
        seq_t *row = (seq_t *)calloc(max_row + 1, sizeof(seq_t));
        if (!row)
            abort();
#endif

        if (max_row == 1) {
            row[1] = (seq_t)2;
        } else if (max_row == 2) {
            row[1] = (seq_t)2;
            row[2] = (seq_t)5;
        } else {
            idx_t lp1 = max_row - 1;    /* previous row */
            idx_t lp2 = max_row - 2;    /* row before previous */
            for (idx_t k = 1; k <= max_row; ++k) {
                seq_t val = (seq_t)0;
                if (k <= lp1)
                    val += rows[lp1][k];
                if (k - 1 >= 1 && k - 1 <= lp1)
                    val += (seq_t)3 * rows[lp1][k - 1];
                if (k - 1 >= 1 && k - 1 <= lp2)
                    val -= (seq_t)2 * rows[lp2][k - 1];
                if (k - 2 >= 1 && k - 2 <= lp2)
                    val -= (seq_t)2 * rows[lp2][k - 2];
                row[k] = val;
            }
        }
        rows[max_row] = row;
    }
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* Find row i such that i*(i+1)/2 ≥ n.  Use idx_t arithmetic. */
    idx_t i = 1;
    while ((i * (i + 1)) / 2 < n)
        ++i;

    /* Column index j (1‑based) within that row. */
    idx_t j = n - ((i - 1) * i) / 2;

    /* Compute rows up to i. */
    ensure_rows(i);

    return rows[i][j];
}
