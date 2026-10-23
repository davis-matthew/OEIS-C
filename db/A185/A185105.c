/*
 * A185105  Number T(n,k) of entries in the k-th cycles of all permutations of
 *          {1,2,..,n}; each cycle is written with the smallest element first
 *          and cycles are arranged in increasing order of their first
 *          elements.
 * https://oeis.org/A185105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 78, u64 n <= 190,
 *            u128 n <= 561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A185105
   a(n) is the n-th term of the row‑major list of the 2‑D array
   T(n,k), 1 ≤ k ≤ n:
        T(n,0) = (n+1)!          (used in the recurrence only)
        T(1,1) = 1
        For n > 1:
             T(n,k) = (n-1)*T(n-1,k) + T(n-1,k-1)   (1 ≤ k ≤ n)
        with T(n,n) = 1.
   The first two rows are:
        n=1: 1
        n=2: 3, 1
   and so on as given in the OEIS entry.  The table is accessed by
   the usual order of an integer triangle: first the 1‑term row, then
   the two‑term row, then the three‑term row, etc.
   Initial values (n = 1, …, 24):
        1, 3, 1, 12, 5, 1, 60, 27, 8, 1,
        360, 168, 59, 12, 1, 2520, 1200, 463, 119, 17, 1,
        20160, 9720, 3978
   The function below builds the necessary rows on demand and returns
   a(n) for any 1‑based index n.  It works for all integer precisions
   that the build system provides for seq_t and idx_t. */

#include <stdlib.h>
#include <stddef.h>

static seq_t **table = NULL;          /* array of pointers to rows   */
static size_t  rows_alloc = 0;        /* number of rows stored       */
static size_t  table_cap   = 0;        /* current capacity of 'table' */

/* Keeps track of the factorial (n+1)! for the next row to be generated. */
static seq_t fact_init = (seq_t)1;    /* starts as 0!  */

/* Ensure that rows 1 … r are computed and stored in 'table'. */
static void ensure_rows(idx_t r)
{
    while (rows_alloc < r) {
        idx_t newR = rows_alloc + 1;                /* next row index (1‑based) */

        /* Allocate space for the new row: indices 0 … newR. */
        seq_t *row = (seq_t*)malloc((size_t)(newR + 1) * sizeof(seq_t));
        if (!row) abort();                          /* OOM → abort */

        /* Update the factorial to (newR+1)! */
        fact_init = fact_init * (seq_t)(newR + 1);
        row[0] = fact_init;                        /* T(newR,0) */

        if (newR == 1) {                            /* base row */
            row[1] = (seq_t)1;                      /* T(1,1) */
        } else {
            seq_t *prev = table[rows_alloc - 1];    /* previous row (newR-1) */
            /* Compute T(newR,k) for k = 1 … newR */
            for (idx_t k = 1; k <= newR; ++k) {
                seq_t term1 = (k <= newR - 1)
                    ? prev[k] * (seq_t)(newR - 1)
                    : (seq_t)0;
                seq_t term2 = prev[k - 1];
                row[k] = term1 + term2;
            }
        }

        /* Grow the pointer table if necessary */
        if (rows_alloc >= table_cap) {
            size_t new_cap = table_cap ? table_cap * 2 : 4;
            seq_t **tmp = (seq_t**)realloc(table,
                    new_cap * sizeof(seq_t*));
            if (!tmp) abort();                      /* OOM → abort */
            table = tmp;
            table_cap = new_cap;
        }

        table[rows_alloc] = row;
        ++rows_alloc;
    }
}

/* Return the n‑th element of the OEIS sequence A185105. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* unused, safety */

    /* Find the row r such that triangular(r‑1) < n ≤ triangular(r). */
    idx_t r = 0;
    idx_t tri = 0;                                 /* cumulative count */
    while (tri < n) {
        ++r;
        tri += r;                                  /* tri = r(r+1)/2  */
    }

    /* Position k within this row (1‑based). */
    idx_t k = n - (tri - r);                       /* k = n - triangular(r‑1) */

    ensure_rows(r);

    /* table[r‑1][k] is T(r,k) – the desired value */
    return table[r - 1][k];
}
