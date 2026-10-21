/*
 * A213221  Riordan array (f(x), x*g(x)) where f(x) is the g.f. of A157004 and
 *          g(x) is the g.f. of A157003.
 * https://oeis.org/A213221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 2,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A213221:  flattening of the Riordan array (f(x),x*g(x))
    where f(x) = A157004, g(x) = A157003.
    The entries T(n,k) of the triangle satisfy
        T(0,0) = 1
        T(1,0) = 2 ,  T(1,1) = 1
        for n ≥ 2:
            T(n,0)  = 2·T(n,1) – 2·T(n-2,1)
        for 1 ≤ k ≤ n-1:
            T(n,k)  = T(n-1,k-1) + T(n,k+1) – T(n-2,k+1)
    The sequence a(n) is the triangle written row‑wise:
        a(0)=T(0,0),  a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …   */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"

/* static storage for the computed rows of the triangle */
static seq_t **rows = NULL;
static size_t    rows_count = 0;
static size_t    rows_cap   = 0;

/* create rows up to the given target (inclusive) */
static void ensure_rows_up_to(size_t target)
{
    /* grow the array of row pointers if necessary */
    if (rows == NULL) {
        rows_cap = target + 1;
        rows = (seq_t **)malloc(sizeof(seq_t *) * rows_cap);
    } else if (rows_cap < target + 1) {
        rows_cap = target + 1;
        rows = (seq_t **)realloc(rows, sizeof(seq_t *) * rows_cap);
    }

    /* compute missing rows */
    for (size_t r = rows_count; r <= target; ++r) {
        /* allocate space for the r‑th row */
        seq_t *row = alloc_seq_array(r + 1);

        if (r == 0) {
            row[0] = 1;
        } else if (r == 1) {
            row[0] = 2;
            row[1] = 1;
        } else {
            /* last element */
            row[r] = 1;

            /* intermediate elements k = r-1 … 1 */
            for (size_t k = r - 1; k > 0; --k) {
                seq_t left  = rows[r - 1][k - 1];
                seq_t right = row[k + 1];
                seq_t above = 0;
                if (r > 2 && k + 1 <= r - 2) {
                    above = rows[r - 2][k + 1];
                }
                row[k] = left + right - above;
            }

            /* first element */
            seq_t sec      = row[1];
            seq_t above_sec = rows[r - 2][1];
            row[0] = 2 * sec - 2 * above_sec;
        }

        rows[r] = row;
    }
    rows_count = target + 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* convert index safely */
    size_t idx = (size_t)n;

    /* find the row r that contains the index */
    size_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= idx) {
        ++r;
    }

    /* ensure the required rows are computed */
    ensure_rows_up_to(r);

    /* column within the row */
    size_t start = r * (r + 1) / 2;
    size_t k = idx - start;

    return rows[r][k];
}
