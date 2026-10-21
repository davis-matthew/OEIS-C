/*
 * A073107  Triangle T(n,k) read by rows, where e.g.f. for T(n,k) is
 *          exp((1+y)*x)/(1-x).
 * https://oeis.org/A073107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073107
 *
 * A( n )  ==  T( r , k ) where
 *   r*(r+1)/2      <= n < (r+1)*(r+2)/2
 *   k               = n - r*(r+1)/2
 *
 * The triangle entry satisfies the recurrence
 *
 *      T( r , k ) = r * T( r-1 , k ) + C( r , k )
 *
 * where C( r , k ) are binomial coefficients.
 *
 * Implementation limits the amount of work per call to O( r ) and
 * memoises rows for arbitrary ordering of calls.
 *
 * All arithmetic is performed in the opaque integer type seq_t.
 * Only the operations allowed by the problem statement are used.
 *
 * Author: ChatGPT                                 */

#include <stdint.h>
#include <stdlib.h>

/* --- Integer square‑root for unsigned 64‑bit values ------------------- */
static idx_t
isqrt_u64(idx_t v)
{
    idx_t x = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = ((idx_t)1) << i;
        if (x + bit <= v / (x + bit))
            x += bit;
    }
    return x;
}

/* --- Memoised storage of the triangle -------------------------------- */
static seq_t **rows = NULL;        /* rows[0] points to T(0,0), etc.     */
static idx_t rows_used = 0;        /* number of rows stored (except row 0) */

static seq_t *prev_row = NULL;     /* pointer to previous row values      */
static idx_t prev_row_n = 0;       /* number of elements in prev_row     */

static seq_t *prev_binom = NULL;   /* pointer to previous row binomials  */
static idx_t prev_binom_n = 0;     /* number of elements in prev_binom   */

/* --------------------------------------------------------------------- */
static void
ensure_row(idx_t R)
{
    /* compute rows until we have row R (inclusive) */
    while (rows_used <= R) {
        idx_t n = rows_used;  /* next row to compute */

        /* allocate binomial coefficients for row n */
        seq_t *cur_binom = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cur_binom) exit(1);

        if (n == 0) {
            cur_binom[0] = 1;      /* C(0,0) = 1 */
        } else {
            cur_binom[0] = 1;      /* C(n,0) = 1 */
            cur_binom[n] = 1;      /* C(n,n) = 1 */
            for (idx_t k = 1; k < n; ++k)
                cur_binom[k] = prev_binom[k - 1] + prev_binom[k];
        }

        /* allocate values for current triangle row */
        seq_t *cur_row = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cur_row) exit(1);

        if (n == 0) {
            cur_row[0] = 1;        /* T(0,0) = 1 */
        } else {
            for (idx_t k = 0; k <= n; ++k) {
                seq_t prev_val = (k <= prev_row_n) ? prev_row[k] : 0;
                cur_row[k] = n * prev_val + cur_binom[k];
            }
        }

        /* keep the new row -> store pointer in rows array */
        if (rows_used >= rows_used) {   /* enlarge rows array if needed */
            size_t new_cap = (rows_used == 0) ? 4 : rows_used * 2;
            seq_t **tmp = (seq_t **)
                          realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) exit(1);
            rows = tmp;
        }
        rows[rows_used] = cur_row;

        /* prepare state for next iteration */
        if (prev_binom) free(prev_binom);
        prev_binom      = cur_binom;
        prev_binom_n    = n;

        prev_row      = cur_row;
        prev_row_n    = n;

        rows_used++;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* determine (row, col) index */
    if (n == 0)
        return 1;

    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t k = n - r * (r + 1) / 2;

    /* compute required rows */
    ensure_row(r);

    /* fetch value from memoised triangle */
    return rows[r][k];
}
