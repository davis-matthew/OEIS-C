/*
 * A067347  Square array read by antidiagonals:
 *          T(n,k)=(T(n,k-1)*n^2-Catalan(k-1)*n)/(n-1) with a(n,0)=1 and
 *          a(1,k)=Catalan(k) where Catalan(k)=C(2k,k)/(k+1)=A000108(k).
 * https://oeis.org/A067347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is the nth element of an array read along antidiagonals *
 *            of the square array T, where * T[r][0] = 1 (all r) * T[1][k] =
 *            Catalan(k) * T[r][k] = (T[r][k-1] * r^2 - Catalan(k-1) * r) /
 *            (r-1) for r >= 2 * Catalan(k) = binomial(2k, k) / (k+1) . * The
 *            sequence is read by antidiagonals: T[0,0],...
 * Range:     u8 n <= 30, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---------- cached data ---------- */
    static seq_t **T          = NULL;   /* T[row][col] */
    static idx_t *row_len     = NULL;   /* number of columns already computed in each row */
    static idx_t *row_cap     = NULL;   /* allocated capacity of each row */
    static idx_t T_cap        = 0;      /* number of rows currently allocated */

    static seq_t *catalan     = NULL;   /* Catalan numbers, indexed by k (Catalan(0)=1) */
    static idx_t cat_len      = 0;      /* number of Catalan values computed */
    static idx_t cat_cap      = 0;      /* allocated capacity of catalan array */

    /* ---------- diagonal index (antidiagonal number) ---------- */
    idx_t d = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;

    idx_t r = n - d * (d + 1) / 2;  /* row offset along diagonal          */
    idx_t c = d - r;                /* column index in that row           */

    /* ---------- ensure rows up to r ---------- */
    if (T_cap <= r) {
        idx_t new_cap = r + 1;
        T          = (seq_t**)realloc(T, (size_t)new_cap * sizeof(seq_t*));
        row_len    = (idx_t*)realloc(row_len, (size_t)new_cap * sizeof(idx_t));
        row_cap    = (idx_t*)realloc(row_cap, (size_t)new_cap * sizeof(idx_t));
        for (idx_t i = T_cap; i < new_cap; ++i) {
            T[i] = NULL;
            row_len[i] = 0;
            row_cap[i] = 0;
        }
        T_cap = new_cap;
    }

    /* ---------- ensure Catalan numbers up to c ---------- */
    if (cat_len <= c) {
        idx_t needed = c + 1;
        if (cat_cap < needed) {
            idx_t new_cap = cat_cap ? cat_cap * 2 : 8;
            while (new_cap < needed) new_cap *= 2;
            catalan = (seq_t*)realloc(catalan, (size_t)new_cap * sizeof(seq_t));
            cat_cap = new_cap;
        }
        if (cat_len == 0) {
            catalan[0] = (seq_t)1;
            cat_len = 1;
        }
        for (idx_t k = cat_len; k <= c; ++k) {
            seq_t prev = catalan[k - 1];
            /* mult = 2*(2k-1)   (use uint64_t to avoid overflow for small k) */
            uint64_t tmp = 2 * ((uint64_t)2 * (k - 1) + 1);
            seq_t mult = (seq_t)tmp;
            seq_t next = (prev * mult) / (seq_t)(k + 1);
            catalan[k] = next;
        }
        cat_len = c + 1;
    }

    /* ---------- ensure row r has enough columns ---------- */
    idx_t need_cols = c + 1;
    if (T[r] == NULL) {
        T[r] = (seq_t*)calloc((size_t)need_cols, sizeof(seq_t));
        row_cap[r] = need_cols;
        row_len[r] = 0;
    } else if (need_cols > row_cap[r]) {
        T[r] = (seq_t*)realloc(T[r], (size_t)need_cols * sizeof(seq_t));
        row_cap[r] = need_cols;
    }

    /* ---------- compute missing entries in row r ---------- */
    idx_t start = row_len[r];
    if (start == 0) {
        T[r][0] = (seq_t)1;      /* base: T[r][0] = 1 for all r */
        start = 1;
        row_len[r] = 1;
    }
    for (idx_t col = start; col < need_cols; ++col) {
        if (r == 0) {
            T[r][col] = (seq_t)0;                     /* row 0, k>=1 are 0 */
        } else if (r == 1) {
            T[r][col] = catalan[col];                 /* row 1 uses Catalan(k) */
        } else {
            seq_t prev = T[r][col - 1];
            seq_t num  = prev * (seq_t)r * (seq_t)r - catalan[col - 1] * (seq_t)r;
            seq_t denom = (seq_t)(r - 1);
            T[r][col] = num / denom;                  /* exact division */
        }
    }
    if (row_len[r] < need_cols)
        row_len[r] = need_cols;

    /* ---------- return the required element ---------- */
    return T[r][c];
}
