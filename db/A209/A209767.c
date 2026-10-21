/*
 * A209767  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209768; see the Formula section.
 * https://oeis.org/A209767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(1,x)=1, v(1,x)=1; u(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x),
 *            v(n,x)=2x*u(n-1,x)+(x+1)*v(n-1,x)+1
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for the triangle */
static seq_t **U_rows = NULL;      /* array of pointers to u rows (1‑based) */
static size_t U_rows_cap = 0;      /* allocated capacity of U_rows */
static idx_t   U_rows_cnt = 0;     /* number of rows currently stored */

/* current v row (used for constructing the next row) */
static seq_t *cur_v  = NULL;   /* pointer to the v row of the last computed n */
static idx_t   cur_v_len = 0;  /* length of cur_v */

/* convert triangular index n (1‑based) to row r and column k (0‑based) */
static void idx_to_rowcol(idx_t n, idx_t *r, idx_t *k)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *r = low;
    __int128 prev = (__int128)(low - 1) * low / 2;
    *k = (idx_t)(n - prev - 1);
}

/* ensure that rows 1…cnt are computed and stored */
static void ensure_rows(idx_t cnt)
{
    if (U_rows_cnt >= cnt)
        return;                       /* nothing to do */

    for (idx_t r = U_rows_cnt + 1; r <= cnt; ++r) {
        size_t len = (size_t)r;
        seq_t *new_u = (seq_t *)malloc(len * sizeof(seq_t));
        seq_t *new_v = (seq_t *)malloc(len * sizeof(seq_t));
        if (!new_u || !new_v) {
            /* allocation failure – abort silently */
            abort();
        }

        if (r == 1) {
            /* base row */
            new_u[0] = (seq_t)1;
            new_v[0] = (seq_t)1;
        } else {
            seq_t *prev_u = U_rows[r - 2];    /* u(n‑1) */
            seq_t *prev_v = cur_v;           /* v(n‑1) */

            /* compute new u */
            for (idx_t k = 0; k < len; ++k) {
                seq_t part1 = (k >= 1) ? prev_u[k - 1] : 0;
                seq_t part2 = (k >= 1 ? prev_v[k - 1] : 0) +
                              (k + 1 < len ? prev_v[k] : 0);
                new_u[k] = part1 + part2;
            }

            /* compute new v */
            for (idx_t k = 0; k < len; ++k) {
                seq_t part2 = (k >= 1) ? ((seq_t)2 * prev_u[k - 1]) : 0;
                seq_t part3 = (k >= 1 ? prev_v[k - 1] : 0) +
                              (k + 1 < len ? prev_v[k] : 0);
                new_v[k] = part2 + part3;
                if (k == 0)
                    new_v[0] += (seq_t)1;
            }
        }

        /* store new u row */
        if (U_rows_cnt >= U_rows_cap) {
            size_t new_cap = U_rows_cap ? U_rows_cap * 2 : 4;
            seq_t **tmp = (seq_t **)realloc(U_rows, new_cap * sizeof(seq_t *));
            if (!tmp) abort();
            U_rows = tmp;
            U_rows_cap = new_cap;
        }
        U_rows[U_rows_cnt] = new_u;
        ++U_rows_cnt;

        /* replace current v by the newly created one (free old) */
        if (cur_v)
            free(cur_v);
        cur_v      = new_v;
        cur_v_len = (idx_t)len;
    }
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* safety for n < 1 */

    idx_t row, col;
    idx_to_rowcol(n, &row, &col);
    ensure_rows(row);

    return U_rows[row - 1][(size_t)col];
}
