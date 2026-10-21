/*
 * A051141  Triangle read by rows: a(n, m) = S1(n, m)*3^(n-m), where S1 are
 *          the signed Stirling numbers of first kind A008275 (n >= 1, 1 <= m
 *          <= n).
 * https://oeis.org/A051141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = flattening of the triangle with recurrence a(n,m) =
 *            a(n-1,m-1) - 3*(n-1)*a(n-1,m) for n ≥ m ≥ 1, where a(0,0)=1 and
 *            a(n,0)=0 for n≥1
 * Range:     i8 n <= 6, i16 n <= 21, i32 n <= 45, i64 n <= 120, i128 n <= 351,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ========================================================================= */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* int64_t */
#include <stddef.h>   /* size_t */

static seq_t **rows          = NULL; /* rows[i] holds the (i+1)-th row  */
static size_t  rows_cap      = 0;
static size_t  rows_cnt      = 0;

static size_t *cumlen        = NULL; /* cumulative lengths (flattened) */
static size_t  cumlen_cap    = 0;
static size_t  cumlen_cnt    = 0;

/* ----------------------------------------------------------------- */
static void ensure_rows_capacity(size_t need)
{
    if (need <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof *rows);
    if (!tmp) abort();
    rows     = tmp;
    rows_cap = new_cap;
}

/* ----------------------------------------------------------------- */
static void ensure_cumlen_capacity(size_t need)
{
    if (need <= cumlen_cap) return;
    size_t new_cap = cumlen_cap ? cumlen_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    size_t *tmp = (size_t*)realloc(cumlen, new_cap * sizeof *cumlen);
    if (!tmp) abort();
    cumlen     = tmp;
    cumlen_cap = new_cap;
}

/* ----------------------------------------------------------------- */
/* Compute the next triangle row and update state.                    */
static void compute_next_row(void)
{
    /* New row number (1‑based) */
    size_t n = rows_cnt + 1;

    /* Allocate space for indices 0..n; index 0 is unused so that
       row indices start at 1, matching the recurrence definition. */
    seq_t *cur = (seq_t*)malloc((n + 1) * sizeof *cur);
    if (!cur) abort();

    cur[0] = 0; /* unused */
    if (n == 1) {
        /* Base case: row 1 has only one element */
        cur[1] = 1;
    } else {
        seq_t *prev  = rows[rows_cnt - 1];
        uint32_t fac = 3 * (uint32_t)(n - 1);          /* factor 3*(n-1) */

        for (size_t m = 1; m <= n; ++m) {
            seq_t prev_left  = (m > 1      ? prev[m - 1] : 0);
            seq_t prev_here  = (m <= n-1 ? prev[m]    : 0);
            seq_t prod = prev_here * (seq_t)fac;
            cur[m] = prev_left - prod;
        }
    }

    /* Store the new row */
    ensure_rows_capacity(rows_cnt + 1);
    rows[rows_cnt] = cur;
    rows_cnt++;

    /* Update cumulative lengths: number of terms up to this row */
    size_t new_cum = (cumlen_cnt == 0) ? n : (cumlen[cumlen_cnt - 1] + n);
    ensure_cumlen_capacity(cumlen_cnt + 1);
    cumlen[cumlen_cnt] = new_cum;
    cumlen_cnt++;
}

/* ----------------------------------------------------------------- */
/* Public interface: return the n‑th term (1‑based) of the sequence. */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure we have computed enough rows to cover the requested index */
    while (cumlen_cnt == 0 || cumlen[cumlen_cnt - 1] < n) {
        compute_next_row();
    }

    /* Locate the row that contains the n‑th element */
    size_t r = 0;
    while (r < cumlen_cnt && cumlen[r] < n) ++r;

    size_t prev_cum = (r == 0) ? 0 : cumlen[r - 1];
    size_t m        = n - prev_cum;   /* position within row (1‑based) */

    return rows[r][m];
}
