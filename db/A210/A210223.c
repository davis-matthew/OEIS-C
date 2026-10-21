/*
 * A210223  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210224; see the Formula section.
 * https://oeis.org/A210223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials u(n,x) defined by
 *
 *   u(1,x) = 1
 *   u(n,x) = x*u(n‑1,x) + v(n‑1,x) + 1
 *
 *   v(1,x) = 1
 *   v(n,x) = 2x*u(n‑1,x) + x*v(n‑1,x) + 1
 *
 * The sequence a(n) consists of the flattened rows of the u‑polynomials.
 * Row n has n coefficients (constant term first).  Thus
 *   a(1)=1,
 *   a(2)=2, a(3)=1,
 *   a(4)=2, a(5)=5, a(6)=1, …
 *
 * The implementation memoises all previously computed rows so that
 * a(n) can be obtained in O(n) time (actually linear in the row
 * number) and in O(1) time afterwards.  It works correctly for any
 * order of calls and for any index n ≥ 1. */

#include <stdlib.h>

static idx_t max_row = 0;
static seq_t **u_rows = NULL;
static seq_t **v_rows = NULL;

/* Return the triangular number T(r) = r*(r+1)/2, using idx_t arithmetic. */
static idx_t triangular(idx_t r)
{
    return (r * (r + 1)) / 2;
}

/* Ensure that rows 1..r are computed and stored in u_rows and v_rows. */
static void ensure_rows_up_to(idx_t r)
{
    if (r <= max_row)
        return;

    /* Resize constructor pointer arrays. */
    size_t new_cap = (size_t)(r + 1);         /* 1‑based indexing, so index 0 unused */
    u_rows = (seq_t **)realloc(u_rows, new_cap * sizeof(seq_t *));
    v_rows = (seq_t **)realloc(v_rows, new_cap * sizeof(seq_t *));

    for (idx_t rr = max_row + 1; rr <= r; ++rr) {
        seq_t *u = (seq_t *)calloc((size_t)rr, sizeof(seq_t));
        seq_t *v = (seq_t *)calloc((size_t)rr, sizeof(seq_t));

        if (rr == 1) {
            u[0] = 1;
            v[0] = 1;
        } else {
            idx_t prev = rr - 1;

            /* u[0] = v_prev[0] + 1 */
            u[0] = v_rows[prev][0] + 1;

            /* fill u[1..rr-1] */
            for (idx_t k = 1; k < rr; ++k) {
                seq_t term = u_rows[prev][k - 1];
                if (k < prev)                /* add v_prev[k] if it exists */
                    term += v_rows[prev][k];
                u[k] = term;
            }

            /* v[0] = 1 */
            v[0] = 1;

            /* fill v[1..rr-1] */
            for (idx_t k = 1; k < rr; ++k) {
                v[k] = (seq_t)(2 * u_rows[prev][k - 1] + v_rows[prev][k - 1]);
            }
        }

        u_rows[rr] = u;
        v_rows[rr] = v;
    }

    max_row = r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range; sequence starts at n=1 */

    /* Find the row r containing index n.  Row r satisfies
       triangular(r-1) < n <= triangular(r).  */
    idx_t r = 1;
    while (triangular(r) < n)
        ++r;

    ensure_rows_up_to(r);

    idx_t prev_tri = triangular(r - 1);
    idx_t j = n - prev_tri;          /* 1‑based position in the row */
    return u_rows[r][j - 1];
}
