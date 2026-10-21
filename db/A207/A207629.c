/*
 * A207629  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207630; see the Formula section.
 * https://oeis.org/A207629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) are the coefficients of the polynomials u(n,x) read row by
 *            row. * Recurrence: * u(1,x) = 1, v(1,x) = 1 * u(n,x) = u(n-1,x)
 *            + v(n-1,x) * v(n,x) = (x+1)u(n-1,x) + v(n-1,x) + 1 (n ≥ 2) *
 *            Each a(n) is the constant term, then the coefficient of x, then
 *            x², … of * u(n,x). The cumulative number of...
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Dependencies ---------- */

#include <stdlib.h>
#include <stddef.h>

/* ---------- Global state (memoization) ---------- */

static seq_t **u_rows      = NULL; /* pointers to coefficient arrays of each row */
static size_t *cum_terms   = NULL; /* cumulative number of terms up to each row (inclusive) */
static size_t  rows_cap    = 0;    /* current capacity of the tables */
static size_t  rows_cnt    = 0;    /* number of rows stored */

/* previous two rows of u(n,x), used to compute the next one */
static seq_t *prev1  = NULL;      /* u(n-1,x) */
static seq_t *prev2  = NULL;      /* u(n-2,x) */
static size_t len_prev1 = 0;      /* length of prev1 */
static size_t len_prev2 = 0;      /* length of prev2 */

/* ---------- Helper to append a new row to the memoized tables ---------- */

static void add_row(seq_t *row, size_t len)
{
    if (rows_cnt == rows_cap) {
        size_t new_cap = (rows_cap == 0) ? 4 : rows_cap * 2;
        seq_t **new_rows      = (seq_t **)realloc(u_rows,      new_cap * sizeof(seq_t *));
        size_t *new_cum_terms = (size_t *)realloc(cum_terms,  new_cap * sizeof(size_t));
        if (!new_rows || !new_cum_terms) { /* allocation failure – abort quietly */
            return;
        }
        u_rows     = new_rows;
        cum_terms  = new_cum_terms;
        rows_cap   = new_cap;
    }
    u_rows[rows_cnt] = row;
    if (rows_cnt == 0) {
        cum_terms[rows_cnt] = len;
    } else {
        cum_terms[rows_cnt] = cum_terms[rows_cnt-1] + len;
    }
    rows_cnt++;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* n is 1‑based; guard against calls below the offset */
    if (n == 0) return 0;

    /* ---------- Initialise the first two rows on first entry ---------- */
    if (rows_cnt == 0) {
        /* Row 1: u(1,x) = 1 */
        seq_t *row1 = (seq_t *)malloc(sizeof(seq_t));
        if (!row1) return 0;
        row1[0] = 1;
        add_row(row1, 1);
        prev2 = row1; len_prev2 = 1;

        /* Row 2: u(2,x) = 2 */
        seq_t *row2 = (seq_t *)malloc(sizeof(seq_t));
        if (!row2) return 0;
        row2[0] = 2;
        add_row(row2, 1);
        prev1 = row2; len_prev1 = 1;
    }

    /* ---------- Grow rows until we have at least n terms ---------- */
    while (cum_terms[rows_cnt-1] < (size_t)n) {
        /* length of the next row: max(length(prev1), length(prev2)+1) */
        size_t len_next = (len_prev2 + 1 > len_prev1) ? (len_prev2 + 1) : len_prev1;
        seq_t *next = (seq_t *)calloc(len_next, sizeof(seq_t));
        if (!next) return 0; /* failed allocation */

        /* 2*u(prev1) */
        for (size_t i = 0; i < len_prev1; ++i)
            next[i] += (seq_t)2 * prev1[i];

        /* x*u(prev2) → shift by one position */
        for (size_t i = 0; i < len_prev2; ++i)
            next[i+1] += prev2[i];

        /* constant term 1 */
        next[0] += 1;

        /* store the new row and rotate previous rows */
        add_row(next, len_next);
        prev2 = prev1; len_prev2 = len_prev1;
        prev1 = next;  len_prev1 = len_next;
    }

    /* ---------- Locate the row that contains the requested term ---------- */
    size_t lo = 0, hi = rows_cnt - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum_terms[mid] >= (size_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t row_index = lo;
    size_t prev_cum  = (row_index == 0) ? 0 : cum_terms[row_index-1];
    size_t offset    = (size_t)n - prev_cum - 1;

    return u_rows[row_index][offset];
}
