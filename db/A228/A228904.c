/*
 * A228904  Triangle defined by g.f. A(x,y) = exp( Sum_{n>=1} x^n/n *
 *          Sum_{k=0..n} binomial(n*k, k^2) * y^k ), as read by rows.
 * https://oeis.org/A228904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A228904 – Flattened triangle from the generating function
 *
 *   A(x,y) = exp( Sum_{n>=1} x^n/n * Sum_{k=0..n} binomial(n*k, k^2) * y^k )
 *
 *   The triangle entries are read by rows.  For a given linear index n
 *   the row r is the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
 *   The column is then c = n - r*(r+1)/2.
 *
 *   We compute the triangle by the recurrence
 *
 *        E[0][0] = 1
 *        E[n][k] = (1/n) * Σ_{i=1..n} Σ_{j=0..min(k,i)} binomial(i*j, j^2) * E[n-i][k-j]
 *
 *   where E[n][k] is the coefficient of x^n y^k.  The function
 *   seq_t a(idx_t n) returns the flattened sequence a(n).
 *
 *   The implementation uses dynamic arrays and the helper functions
 *   provided in the build environment: binomial, seq_alloc_new_array,
 *   seq_free_array.  All intermediate calculations are done in
 *   seq_t, and the division by n is exact by construction.
 *
 *   The algorithm is O(R^3) where R is the maximum row number
 *   needed; for indices up to 10 000 this is R≈141 and easily
 *   below the time limit. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* ---------- triangle rows (E[n][k]) ---------- */
static seq_t **E_rows = NULL;    /* array of pointers, each pointing to a row of length r+1 */
static size_t E_cap   = 0;       /* capacity of E_rows array */
static size_t E_cnt   = 0;       /* number of rows currently computed */

/* ---------- pre‑computed tables T_i[j] = binomial(i*j, j^2) ---------- */
static seq_t **T_tables = NULL;  /* T_tables[i-1] holds table for i (i≥1) */
static size_t T_cap    = 0;      /* number of tables currently computed (i.e., max i) */

/* ---------- helper to grow E_rows array ---------- */
static void ensure_E_rows_cap(size_t needed)
{
    if (needed <= E_cap) return;
    size_t new_cap = E_cap ? E_cap << 1 : 8;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t **new_arr = (seq_t **)realloc(E_rows, new_cap * sizeof(seq_t *));
    if (!new_arr) exit(1);
    for (size_t i = E_cap; i < new_cap; ++i) new_arr[i] = NULL;
    E_rows = new_arr;
    E_cap  = new_cap;
}

/* ---------- helper to grow T_tables array ---------- */
static void ensure_T_tables_cap(size_t needed)
{
    if (needed <= T_cap) return;
    size_t new_cap = T_cap ? T_cap << 1 : 8;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t **new_arr = (seq_t **)realloc(T_tables, new_cap * sizeof(seq_t *));
    if (!new_arr) exit(1);
    for (size_t i = T_cap; i < new_cap; ++i) new_arr[i] = NULL;
    T_tables = new_arr;
    T_cap    = new_cap;
}

/* ---------- compute single table T_i (length i+1) ---------- */
static seq_t *compute_T_table(idx_t i)
{
    size_t len = (size_t)(i + 1);
    seq_t *tbl = seq_alloc_new_array(len);
    for (size_t r = 0; r < len; ++r) tbl[r] = 0;   /* ensure zero init if not already */
    for (idx_t j = 0; j <= i; ++j)
        tbl[(size_t)j] = binomial((idx_t)(i * j), (idx_t)(j * j));
    return tbl;
}

/* ---------- ensure triangle rows up to rmax are computed ---------- */
static void ensure_rows(idx_t rmax)
{
    if (E_cnt > 0 && rmax < (idx_t)(E_cnt - 1)) return;

    /* make sure we have storage for all requested rows */
    ensure_E_rows_cap((size_t)(rmax + 1));

    /* make sure tables T_i up to rmax exist */
    ensure_T_tables_cap((size_t)(rmax + 1));

    /* compute missing rows */
    for (size_t r = E_cnt; r <= (size_t)rmax; ++r) {
        if (r == 0) {          /* base row */
            seq_t *row0 = seq_alloc_new_array(1);
            row0[0] = 1;
            E_rows[0] = row0;
            continue;
        }

        /* allocate current row, zeroed */
        size_t rowlen = r + 1;
        seq_t *row = seq_alloc_new_array(rowlen);
        for (size_t i = 0; i < rowlen; ++i) row[i] = 0;

        /* pre‑compute tables T_i on demand */
        for (size_t i = 1; i <= r; ++i) {
            if (T_tables[i - 1] == NULL)
                T_tables[i - 1] = compute_T_table((idx_t)i);
        }

        /* convolution of each T_i with previous row E[r-i] */
        for (size_t i = 1; i <= r; ++i) {
            const seq_t *Ti = T_tables[i - 1];
            size_t Ti_len = i + 1;
            const seq_t *prev = E_rows[r - i];
            size_t prev_len = (r - i) + 1;

            for (size_t a = 0; a < Ti_len; ++a) {
                seq_t Ti_a = Ti[a];
                if (Ti_a == 0) continue;
                for (size_t b = 0; b < prev_len; ++b) {
                    row[a + b] += Ti_a * prev[b];
                }
            }
        }

        /* divide by r (exact integer division) */
        seq_t div = (seq_t)r;
        for (size_t k = 0; k < rowlen; ++k) row[k] /= div;

        E_rows[r] = row;
    }
    E_cnt = (size_t)(rmax + 1);
}

/* ---------- convert linear index n to (row, col) ---------- */
static void index_to_row_col(idx_t n, idx_t *rowp, idx_t *colp)
{
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    *rowp = r;
    *colp = n - r * (r + 1) / 2;
}

/* ---------- public function required by the test harness ---------- */
seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_row_col(n, &row, &col);
    ensure_rows(row);
    return E_rows[(size_t)row][(size_t)col];
}
