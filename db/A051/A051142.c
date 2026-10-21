/*
 * A051142  Triangle read by rows: T(n, m) = S1(n, m)*4^(n-m), where S1 are
 *          the signed Stirling numbers of first kind A008275 (n >= 1, 1 <= m
 *          <= n).
 * https://oeis.org/A051142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the element of the triangle T(n,m) * where T(0,0)=1 and
 *            for n>=1,m>=1 * T(n,m) = T(n-1,m-1) - 4*(n-1)*T(n-1,m) * The
 *            sequence a(n) lists the triangle row‑wise starting at n=1
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 36, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#else
#include <new>
#include <cstdlib>
#include <cstring>
#endif

/* Helper to allocate and zero‑initialize an array of seq_t */
static seq_t *alloc_row(size_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz]();   /* value‑initialized */
#else
    return (seq_t*)calloc(sz, sizeof(seq_t));
#endif
}

/* Dynamic array of rows; each row is a seq_t* */
static seq_t **rows = NULL;          /* array of pointers to rows */
static size_t rows_cnt = 0;          /* number of rows stored (including row 0) */
static size_t rows_cap = 0;          /* capacity of the above array */

/* Ensure the rows array has capacity for at least 'need' rows */
static void rows_ensure(size_t need)
{
    if (rows_cap >= need) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while (new_cap <= need) new_cap *= 2;
#ifdef __cplusplus
    seq_t **tmp = new seq_t*[new_cap];
    if (rows) {
        memcpy(tmp, rows, rows_cap * sizeof(seq_t*));
        delete[] rows;
    }
    rows = tmp;
#else
    seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
    rows = tmp;                       /* assume success in the test harness */
#endif
    rows_cap = new_cap;
}

/* Ensure that row 'r' (0‑based) is computed and stored */
static void ensure_row(size_t r)
{
    if (rows_cnt == 0) {
        /* Create base row 0: T(0,0)=1 */
        rows_ensure(1);
        rows[0] = alloc_row(1);
        rows[0][0] = (seq_t)1;
        rows_cnt = 1;
    }
    while (rows_cnt <= r) {
        size_t i = rows_cnt;                 /* compute row i (1‑based) */
        size_t sz = i + 1;                   /* indices 0..i */
        seq_t *cur = alloc_row(sz);
        cur[0] = (seq_t)0;
        seq_t factor = (seq_t)(4 * (i - 1)); /* 4*(i-1) fits in seq_t for needed limits */
        for (size_t j = 1; j <= i; ++j) {
            seq_t a = rows[i-1][j-1];
            seq_t b = (j <= i-1) ? rows[i-1][j] : (seq_t)0;
            cur[j] = a - factor * b;
        }
        rows_ensure(rows_cnt + 1);
        rows[rows_cnt] = cur;
        ++rows_cnt;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, return 0 safely */

    /* Find row r such that r*(r+1)/2 >= n */
    size_t r = 1;
    while ( (r * (r + 1)) / 2 < n ) ++r;

    /* Determine column m  (1‑based) */
    size_t start = r * (r - 1) / 2 + 1;
    size_t m = (size_t)(n - start + 1);

    /* Build rows up to r */
    ensure_row(r);

    /* Return the triangle value */
    return rows[r][m];
}
