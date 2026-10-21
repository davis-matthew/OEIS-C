/*
 * A209418  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209417; see the Formula section.
 * https://oeis.org/A209418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangular array * T(n,k) = v(n,k)
 *            defined by the recurrences * * u(1,x) = 1, v(1,x) = 1 * u(n,x) =
 *            x*u(n‑1,x) + v(n‑1,x) * v(n,x) = (x+1)*u(n‑1,x) + 2*x*v(n‑1,x) *
 *            * The array is read by rows: T(1,0); T(2,0),T(2,1);
 *            T(3,0),T(3,1),T(3,2); … * This generates the sequen...
 * Range:     u8 n <= 25, u16 n <= 84, u32 n <= 266, u64 n <= 931,
 *            u128 n <= 1275, big any n
 * Helpers:   flat_index_to_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers (both C and C++). */
#include <stdlib.h>
#include <math.h>
#include "../utils/flat_index_to_rc.h"

/* Memoised storage for the polynomials u(n,·) and v(n,·). */
static seq_t **urows = NULL;   /* urows[1] .. urows[max_row] */
static seq_t **vrows = NULL;   /* vrows[1] .. vrows[max_row] */
static idx_t max_row = 0;

/* Allocate and initialise rows up to 'target' (inclusive). */
static void ensure_rows(idx_t target)
{
    if (target <= max_row)
        return;

    /* Resize pointer arrays. */
    urows = (seq_t**)realloc(urows, (target + 1) * sizeof(seq_t*));
    vrows = (seq_t**)realloc(vrows, (target + 1) * sizeof(seq_t*));

    for (idx_t n = max_row + 1; n <= target; ++n) {
        idx_t len = n;                /* number of coefficients in row n */
        seq_t *new_u, *new_v;

#if defined(__cplusplus)
        /* C++: value‑initialise the array elements. */
        new_u = new seq_t[len]();
        new_v = new seq_t[len]();
#else
        /* C: zero‑initialise the array elements. */
        new_u = calloc(len, sizeof(seq_t));
        new_v = calloc(len, sizeof(seq_t));
#endif

        if (n == 1) {
            new_u[0] = 1;
            new_v[0] = 1;
        } else {
            seq_t *prev_u = urows[n - 1];
            seq_t *prev_v = vrows[n - 1];
            idx_t prev_len = n - 1;

            /* u(n,x) = x*u(n‑1,x) + v(n‑1,x) */
            for (idx_t k = 0; k < len; ++k) {
                seq_t part_u = (k > 0) ? prev_u[k - 1] : 0;
                seq_t part_v = (k < prev_len) ? prev_v[k] : 0;
                new_u[k] = part_u + part_v;
            }

            /* v(n,x) = (x+1)*u(n‑1,x) + 2*x*v(n‑1,x) */
            for (idx_t k = 0; k < len; ++k) {
                seq_t term1 = (k < prev_len) ? prev_u[k] : 0;
                seq_t term2 = (k > 0) ? prev_u[k - 1] : 0;
                seq_t term3 = (k > 0) ? (2 * prev_v[k - 1]) : 0;
                new_v[k] = term1 + term2 + term3;
            }
        }

        urows[n] = new_u;
        vrows[n] = new_v;
    }

    max_row = target;
}

/* Public function: returns the nth term of A209418. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* behaviour below the offset is irrelevant */

    idx_t row, col;
    flat_index_to_rc(n, &row, &col);

    ensure_rows(row);                 /* make sure we have row */

    return vrows[row][col];
}
