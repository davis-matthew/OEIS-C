/*
 * A157343  Triangle T(n,k) read by rows: T(n,k)=1 if the row index n is prime
 *          or <=1, otherwise T(n,k) = T(n-1,k)+T(n-1,k-1).
 * https://oeis.org/A157343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th term of the flattened triangle T(n,k)
    defined in OEIS A157343:

        T(n,k) = 1  if n is prime or n ≤ 1
        T(n,k) = T(n-1,k) + T(n-1,k-1)   otherwise

    The sequence a(n) is obtained by reading the triangle by rows,
    i.e. a(n) = the element with index k in row r, where
    r is the smallest integer with n < (r+1)(r+2)/2.
    The file format guarantees that a(0) = 1.  The function
    works for any n ≥ 0 and uses memoization to reuse previously
    computed rows.  It runs in O(√n) time per call for typical
    n up to several thousand. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

static seq_t **triangle = NULL;          /* array of pointers to rows */
static size_t  rows_allocated = 0;        /* capacity of triangle array   */
static size_t  rows_count = 0;            /* number of rows currently built */

/* Build rows of the triangle up to index 'up_to' (inclusive). */
static void ensure_triangle_rows(idx_t up_to)
{
    while (rows_count <= up_to) {
        idx_t r = (idx_t)rows_count;                    /* new row index   */
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!row) abort(); /* OOM protection */

        /* first and last elements are always 1 */
        row[0] = (seq_t)1;
        if (r > 0) row[r] = (seq_t)1;

        if (r <= 1 || is_prime(r)) {                 /* prime or ≤1 : all 1s */
            for (idx_t k = 1; k < r; ++k) row[k] = (seq_t)1;
        } else {                                       /* composite: Pascal addition */
            for (idx_t k = 1; k < r; ++k) {
                row[k] = triangle[r-1][k] + triangle[r-1][k-1];
            }
        }

        /* enlarge triangle array if necessary */
        if (rows_count == rows_allocated) {
            size_t new_cap = rows_allocated ? rows_allocated * 2 : 4;
            seq_t **tmp = (seq_t **)malloc(new_cap * sizeof(seq_t *));
            if (!tmp) abort();
            if (rows_allocated) memcpy(tmp, triangle, rows_allocated * sizeof(seq_t *));
            free(triangle);
            triangle = tmp;
            rows_allocated = new_cap;
        }

        triangle[rows_count++] = row;
    }
}

/*  OEIS A157343: flattened triangle term a(n).  */
seq_t a(idx_t n)
{
    /* Determine the row r that contains index n.
       row r has r+1 elements, cumulative count up to row r-1 is r*(r+1)/2. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative + r + 1 <= n) {
        cumulative += r + 1;
        ++r;
    }
    idx_t k = n - cumulative;          /* column within row r   */

    ensure_triangle_rows(r);            /* make sure rows 0..r are computed */
    return triangle[r][k];
}
