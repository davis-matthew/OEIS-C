/*
 * A134426  Triangle read by rows: T(n,k) is the number of paths of length n
 *          in the first quadrant, starting at the origin, ending at height k
 *          and consisting of 2 kind of upsteps U=(1,1) (U1 and U2), 3 kind of
 *          flatsteps F=(1,0) (F1, F2 and F3) and 1 kind of downsteps
 *          D=(1,-1).
 * https://oeis.org/A134426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS A134426: the triangle entry a(n).
    a(n) is the (row r, column k) entry of the triangle T defined by:
        T(0,0) = 1
        T(m,k) = 2*T(m-1,k-1) + 3*T(m-1,k) + T(m-1,k+1)   (m>0, k>=0)
    The linear index n is mapped to row and column by:
        r = the unique integer with r(r+1)/2 <= n < (r+1)(r+2)/2
        k = n - r(r+1)/2
    The function memoises all rows up to the requested height in static
    storage so that repeated calls (even out of order) are efficient. */

#include <stdlib.h>

/* Forward declaration of helper that ensures all rows up to 'up_to' exist */
static void ensure_rows(idx_t up_to);

/* Global storage for the triangle: an array of pointers,
   each pointing to a row array of seq_t of length (row+1).  */

static seq_t **triangle = NULL;       /* array of row pointers */
static idx_t triangle_cap = 0;        /* current capacity of 'triangle' array */
static idx_t triangle_used = 0;       /* number of rows currently stored */

/*                                                                         */
/*  Ensure that all rows up to index 'up_to' (inclusive) are computed and    */
/*  stored in 'triangle'.  This function is called by a() whenever a more  */
/*  distant row is needed.                                                  */
/*                                                                         */
static void ensure_rows(idx_t up_to)
{
    if (triangle == NULL) {
        /* Initialise with some capacity; will grow as needed. */
        triangle_cap = 16;
        triangle = (seq_t **)malloc(triangle_cap * sizeof(seq_t *));
        triangle_used = 0;
    }

    for (idx_t i = triangle_used; i <= up_to; ++i) {
        idx_t len = i + 1;
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));

        if (i == 0) {
            /* Base row: T(0,0)=1 */
            curr[0] = (seq_t)1;
        } else {
            seq_t *prev = triangle[i - 1];
            idx_t prev_len = i;   /* length of previous row */

            for (idx_t k = 0; k <= i; ++k) {
                seq_t up   = (k >= 1) ? ((seq_t)2 * prev[k - 1]) : (seq_t)0;
                seq_t flat = (k < prev_len) ? ((seq_t)3 * prev[k]) : (seq_t)0;
                seq_t down = (k + 1 < prev_len) ? prev[k + 1] : (seq_t)0;
                curr[k] = up + flat + down;
            }
        }

        /* Store the newly computed row */
        triangle[i] = curr;
        triangle_used = i + 1;
    }

    /* Ensure capacity for future rows */
    if (triangle_used > triangle_cap) {
        size_t new_cap = triangle_used * 2;
        seq_t **tmp = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
        if (tmp) {
            triangle = tmp;
            triangle_cap = new_cap;
        }
    }
}

/*                                                                         */
/*  Main function: returns a(n).  It maps n to (r,k), ensures the row is   */
/*  available, and returns the appropriate triangle entry.                */
/*                                                                         */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column within that row */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Make sure rows up to r have been computed */
    if (r >= triangle_used)
        ensure_rows(r);

    return triangle[r][k];
}
