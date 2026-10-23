/*
 * A165675  Triangle read by rows. T(n, k) = (n - k + 1)! * H(k, n - k), where
 *          H are the hyperharmonic numbers. For 0 <= k <= n.
 * https://oeis.org/A165675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A165675 – triangle read by rows.
 *
 * T(0,0) = 1.
 * For n ≥ 1:
 *   T(n,0) = n!
 *   T(n,n) = 1
 *   1 ≤ k ≤ n-1:
 *     T(n,k) = (n-k+1) * T(n-1,k) + T(n-1,k-1)
 *
 * a(n) = T(r,k) where r is the unique row with r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *        and k = n - r(r+1)/2.
 *
 * The implementation builds the triangle lazily and caches all rows
 * that have been requested.  All arithmetic is performed with the
 * generic type `seq_t`, so the code works for both built-in integers
 * and arbitrary‑precision integer classes.
 */

#include <stdlib.h>

/* Dynamically allocated cache of rows.  triangle[i] points to an array
 * of length (i+1) containing T(i,0)…T(i,i). */
static seq_t **triangle = NULL;
/* Number of rows that have been cached (rows 0 … triangle_rows-1). */
static size_t triangle_rows = 0;
/* Last computed factorial i!.  It is updated as new rows are generated. */
static seq_t fact = (seq_t)1;

/* Ensure that all rows 0 … r inclusive are cached.  */
static void ensure_triangle(idx_t r)
{
    if (r < (idx_t)triangle_rows)
        return;

    size_t new_rows = (size_t)r + 1;
    seq_t **tmp = (seq_t **)realloc(triangle, new_rows * sizeof(seq_t *));
    if (!tmp)   /* allocation failure – leave cache unchanged */
        return;
    triangle = tmp;

    for (size_t i = triangle_rows; i < new_rows; ++i)
    {
        /* Allocate the current row.  */
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row)   /* allocation failure – leave cache unchanged */
            return;

        if (i == 0) /* first row: 0! = 1 */
        {
            row[0] = (seq_t)1;
            fact = row[0];
        }
        else
        {
            fact = fact * (seq_t)i;        /* i!  */
            row[0] = fact;                 /* T(i,0) */

            /* Interior entries.  */
            for (size_t k = 1; k < i; ++k)
            {
                seq_t term1 = ((seq_t)(i - k + 1)) * triangle[i - 1][k];
                seq_t term2 = triangle[i - 1][k - 1];
                row[k] = term1 + term2;    /* T(i,k) */
            }

            row[i] = (seq_t)1;             /* T(i,i) = 1 */
        }

        triangle[i] = row;
    }

    triangle_rows = new_rows;
}

/* Return the n-th term of A165675, 0‑indexed.  */
seq_t a(idx_t n)
{
    /* Determine the row r that contains position n.  */
    idx_t sum = 0;
    idx_t r   = 0;
    while (sum + r + 1 <= n)
    {
        sum += r + 1;
        ++r;
    }

    idx_t k = n - sum;           /* column within the row  */
    ensure_triangle(r);          /* build rows up to r if needed */
    return triangle[r][k];
}
