/*
 * A209705  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209706; see the Formula section.
 * https://oeis.org/A209705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A209705 – Triangle of coefficients of polynomials u(n,x)
 *
 *  The values are given by the triangle T(n,k) defined for
 *    0 ≤ k < n
 *
 *      T(1,0) = 1
 *      T(2,0) = 0 ,  T(2,1) = 2
 *      T(3,0) = 0 ,  T(3,1) = 3 ,  T(3,2) = 4
 *      T(n,k) = 2*T(n‑1,k) + 2*T(n‑1,k‑1)
 *               – T(n‑2,k) – 2*T(n‑2,k‑1)
 *
 *  a(n) is the row‑major listing of the triangle:
 *      a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), … .
 *
 *  The function below builds the triangle on demand and returns the
 *  requested term.  All arithmetic is performed in type seq_t, so the
 *  result is exact whenever it fits in seq_t.  The implementation
 *  protects all array indices – the previous version failed to
 *  terminate the use of T(n‑2,k‑1) at the end of a row, causing
 *  unsigned underflows.  This version corrects that bound check.
 */

#include <stdlib.h>

static seq_t **triangle = NULL;    /* triangle[r-1] holds row r (0‑based) */
static size_t triangle_rows = 0;   /* number of rows currently built */

/* Build rows 1..needed (inclusive) of the triangle. */
static void ensure_triangle_rows(size_t needed)
{
    if (triangle_rows >= needed)
        return;

    /* Resize the array of row pointers. */
    seq_t **new_tri = (seq_t **)realloc(triangle, needed * sizeof(seq_t *));
    if (!new_tri) {
        abort();                     /* allocation failure */
    }
    triangle = new_tri;

    /* Create any new rows. */
    for (size_t i = triangle_rows; i < needed; ++i) {
        size_t r   = i + 1;          /* row number (1‑based) */
        seq_t *row = (seq_t *)malloc(r * sizeof(seq_t));
        if (!row) abort();
        triangle[i] = row;

        /* Base cases */
        if (r == 1) {
            row[0] = 1;
            continue;
        }
        if (r == 2) {
            row[0] = 0;
            row[1] = 2;
            continue;
        }
        if (r == 3) {
            row[0] = 0;
            row[1] = 3;
            row[2] = 4;
            continue;
        }

        /* Recurrence for r ≥ 4.
         *   prev1  = row r-1   (length r-1)
         *   prev2  = row r-2   (length r-2)
         */
        seq_t *prev1 = triangle[i - 1];   /* row r-1   */
        seq_t *prev2 = triangle[i - 2];   /* row r-2   */

        for (size_t k = 0; k < r; ++k) {
            seq_t t1   = (k < r - 1)           ? prev1[k] : 0;
            seq_t t1a  = (k > 0)                ? prev1[k - 1] : 0;
            seq_t t2   = (k < r - 2)           ? prev2[k] : 0;
            seq_t t2a  = (k > 0 && k < r - 1) ? prev2[k - 1] : 0;
                                    /* the last condition ensures
                                      k-1 < r-2 (i.e. k < r-1) */

            seq_t two_t1   = t1 + t1;
            seq_t two_t1a  = t1a + t1a;
            seq_t two_t2a  = t2a + t2a;

            seq_t res = two_t1 + two_t1a; /* 2*T(n‑1,k) + 2*T(n‑1,k‑1) */
            res -= t2;                    /* – T(n‑2,k) */
            res -= two_t2a;                /* – 2*T(n‑2,k‑1) */
            row[k] = res;
        }
    }

    triangle_rows = needed;
}

/* Return a(n) for n ≥ 1.  The function supports arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety for out‑of‑range calls */
        return 0;

    /* Determine the row r containing the n‑th element:
     *   r satisfies r*(r+1)/2 ≥ n  and (r‑1)*r/2 < n
     */
    size_t r = 1;
    while ((idx_t)r * (r + 1) / 2 < n)
        ++r;

    /* Column index k (0‑based) within row r. */
    idx_t prev_triangular = (idx_t)r * (r - 1) / 2;
    size_t k = ((size_t)n) - (size_t)prev_triangular - 1;

    /* Build the triangle up to row r. */
    ensure_triangle_rows(r);

    return triangle[r - 1][k];
}
