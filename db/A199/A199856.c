/*
 * A199856  Triangle T(n,k), read by rows, given by
 *          (-1,3,0,0,0,0,0,0,0,0,0,...) DELTA
 *          (1,0,-1/3,1/3,0,0,0,0,0,0,0,...) where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A199856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 44, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A199856:  Triangle T(n,k) read by rows.
    Recurrence (n>2):  T(n,k)=2*T(n-1,k)+T(n-1,k-1)-T(n-2,k-1).
    Base rows:
        T(0,0)=1
        T(1,0)=-1, T(1,1)=1
        T(2,0)=-2, T(2,1)=1, T(2,2)=1
    Row k=0 for n>0:  T(n,0) = -2^(n-1).
    The flattened sequence a(n) is the row-major enumeration
    of the triangle. */

#include <stdlib.h>   /* malloc, realloc */

/* Compute 2^exp as seq_t.  exp will not exceed the bit width of seq_t
   for the test cases. */
static inline seq_t pow2_idx(idx_t exp)
{
    return (seq_t)((unsigned long long)1 << exp);
}

/* --- Triangle storage and construction --------------------------------- */

static seq_t **triangle = NULL; /* array of row pointers          */
static idx_t   rows_used = 0;   /* number of rows constructed     */

/* Ensure that rows 0..r_need are computed and stored. */
static void ensure_rows(idx_t r_need)
{
    if (rows_used > r_need) {
        return;
    }

    /* Grow the triangle pointer array if needed. */
    if (triangle == NULL) {
        /* allocate capacity for at least r_need+1 rows */
        triangle = (seq_t **)malloc((size_t)(r_need + 1) * sizeof(seq_t *));
    } else {
        triangle = (seq_t **)realloc(triangle,
                                     (size_t)(r_need + 1) * sizeof(seq_t *));
    }

    /* Construct new rows. */
    for (idx_t n = rows_used; n <= r_need; ++n) {
        size_t len = (size_t)(n + 1);
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));

        if (n == 0) {
            /* T(0,0) */
            row[0] = (seq_t)1;
        } else if (n == 1) {
            /* T(1,0) = -1, T(1,1) = 1 */
            row[0] = (seq_t)-1;
            row[1] = (seq_t)1;
        } else if (n == 2) {
            /* Explicitly set the first three entries. */
            row[0] = (seq_t)-2;
            row[1] = (seq_t)1;
            row[2] = (seq_t)1;
        } else {
            /* n >= 3 */
            /* First entry: T(n,0) = -2^(n-1) */
            row[0] = -pow2_idx(n - 1);

            /* Remaining entries via the recurrence. */
            for (idx_t k = 1; k <= n; ++k) {
                seq_t a = (k <= n - 1) ? triangle[n - 1][k] : (seq_t)0;
                seq_t b = triangle[n - 1][k - 1];
                seq_t c = (k - 1 <= n - 2) ? triangle[n - 2][k - 1] : (seq_t)0;
                row[k] = (seq_t)(2 * a + b - c);
            }
        }

        triangle[n] = row;
    }

    rows_used = r_need + 1;
}

/* --- Public sequence function ------------------------------------------ */

seq_t a(idx_t n)
{
    /* Find the row that contains index n.  Let r be the first row such
       that (r+1)(r+2)/2 > n.  The cumulative count before row r is
       r*(r+1)/2. */
    idx_t r = 0;
    while (((idx_t)(r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t col = n - (idx_t)r * (r + 1) / 2;

    ensure_rows(r);
    return triangle[r][col];
}
