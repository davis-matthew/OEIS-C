/*
 * A208913  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208914; see the Formula section.
 * https://oeis.org/A208913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials u(n,x).
 * Let u(1,x)=1, v(1,x)=1 and for n>1
 *     u(n,x) = u(n-1,x) + 2x*v(n-1,x)
 *     v(n,x) = 2x*u(n-1,x) + v(n-1,x) + 1
 * The sequence a(n) is the flattened triangle obtained by concatenating
 * the rows of u(n,x).  Off‑setting 1: a(1)=1.  The following routine
 * returns a(n) for any n≥1, with memoised rows up to the maximal
 * required r.  All arithmetic is performed with the type seq_t, the
 * native integer type provided by the build. */
#include <stdlib.h>
#include <stdint.h>

/* Tables of coefficients: u_tbl[row][col], v_tbl[row][col]   (row≥1, col<row) */
static seq_t **u_tbl = NULL;
static seq_t **v_tbl = NULL;
static idx_t  max_row = 0;          /* largest row that has been built */

static void ensure_rows(idx_t row)
{
    if (row <= max_row)
        return;

    /* Grow the array of row pointers, if necessary */
    idx_t new_cap = row + 1;                    /* index 0 unused, rows 1..row */
    if (u_tbl == NULL) {
        u_tbl = (seq_t **)calloc(new_cap, sizeof(seq_t *));
        v_tbl = (seq_t **)calloc(new_cap, sizeof(seq_t *));
    } else {
        u_tbl = (seq_t **)realloc(u_tbl, new_cap * sizeof(seq_t *));
        v_tbl = (seq_t **)realloc(v_tbl, new_cap * sizeof(seq_t *));
    }

    /* Initialise the first row, if it has not been done yet */
    if (max_row == 0) {
        u_tbl[1] = (seq_t *)calloc(1, sizeof(seq_t));
        v_tbl[1] = (seq_t *)calloc(1, sizeof(seq_t));
        u_tbl[1][0] = (seq_t)1;
        v_tbl[1][0] = (seq_t)1;
        max_row = 1;
    }

    /* Build subsequent rows up to ‘row’ */
    for (idx_t r = max_row + 1; r <= row; ++r) {
        seq_t *u_prev = u_tbl[r - 1];
        seq_t *v_prev = v_tbl[r - 1];
        idx_t len_cur = r;                     /* number of coefficients in row r */

        seq_t *u_cur = (seq_t *)calloc(len_cur, sizeof(seq_t));
        seq_t *v_cur = (seq_t *)calloc(len_cur, sizeof(seq_t));

        /* Constant term (k = 0) */
        u_cur[0] = u_prev[0];
        v_cur[0] = v_prev[0] + (seq_t)1;

        /* Remaining coefficients (k = 1 .. len_cur-1) */
        for (idx_t k = 1; k < len_cur; ++k) {
            seq_t a = (k < len_cur - 1) ? u_prev[k] : (seq_t)0;
            seq_t b = (seq_t)2 * v_prev[k - 1];
            u_cur[k] = a + b;

            seq_t c = (k < len_cur - 1) ? v_prev[k] : (seq_t)0;
            seq_t d = (seq_t)2 * u_prev[k - 1];
            v_cur[k] = c + d;
        }

        u_tbl[r] = u_cur;
        v_tbl[r] = v_cur;
    }

    max_row = row;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    ensure_rows(r);

    /* The first index of row r is j0 = (r-1)r/2 + 1.  */
    idx_t j0 = (r * (r - 1)) / 2 + (idx_t)1;
    idx_t k = n - j0;             /* zero‑based column index (0 ≤ k < r) */

    return u_tbl[r][k];
}
