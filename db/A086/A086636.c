/*
 * A086636  Triangle of coefficients, read by rows, where T(n,k) is the
 *          coefficient of x^n*y^k in f(x,y) that satisfies f(x,y) =
 *          (3-sqrt(1-4x))/2 + xy*f(x,y)^3.
 * https://oeis.org/A086636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086636:  Triangle of coefficients read by rows,
   where T(n,k) is the coefficient of x^n*y^k in
   f(x,y) = (3 - sqrt(1-4x))/2 + xy*f(x,y)^3.  */
/* This implementation builds the triangle incrementally and
   memoizes it.  The first column contains Catalan numbers
   (offset by one) and the main diagonal contains A001764. */
/* External types supplied by the build */
extern seq_t a(idx_t n);

/* ----------  required helpers from the utils package  ---------- */
#include "../utils/catalan.h"      /* catalan(idx_t)            */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#ifndef EXIT_FAILURE
#  define EXIT_FAILURE 1
#endif

/* ----------  internal state  ---------- */
static seq_t **tri = NULL;          /* pointer to rows                */
static size_t cur_rows = 0;         /* number of rows already built   */
static seq_t *Cval = NULL;          /* array of C(n) values           */
static size_t C_cap = 0;            /* capacity of Cval array         */
static seq_t *conv = NULL;          /* temporary buffer for convolutions */
static size_t conv_cap = 0;         /* capacity of conv buffer        */

/* ----------  make sure rows 0..needed are computed  ---------- */
static void ensure_rows(idx_t needed)
{
    if (needed < cur_rows) return;      /* already available */

    /* --- grow triangle array --- */
    size_t new_rows = (size_t)needed + 1;
    tri = (seq_t**)realloc(tri, new_rows * sizeof(seq_t*));
    if (!tri) { exit(EXIT_FAILURE); }

    /* --- grow C array --- */
    if (new_rows > C_cap) {
        Cval = (seq_t*)realloc(Cval, new_rows * sizeof(seq_t));
        if (!Cval) { exit(EXIT_FAILURE); }
        C_cap = new_rows;
    }

    /* --- grow convolution buffer --- */
    if (new_rows > conv_cap) {
        conv = (seq_t*)realloc(conv, new_rows * sizeof(seq_t));
        if (!conv) { exit(EXIT_FAILURE); }
        conv_cap = new_rows;
    }

    /* --- build missing rows --- */
    for (size_t n = cur_rows; n < new_rows; ++n) {
        size_t len = n + 1;
        tri[n] = (seq_t*)calloc(len, sizeof(seq_t));   /* zero-initialised */
        if (!tri[n]) { exit(EXIT_FAILURE); }

        /* compute C(n) : C(0)=1, C(n)=Catalan(n-1) for n>=1 */
        if (n == 0) Cval[n] = (seq_t)1;
        else        Cval[n] = catalan((idx_t)(n-1));
        tri[n][0] = Cval[n];

        if (n == 0) continue; /* no convolution for the first row */

        /* --- convolution part: xy*f^3 --- */
        for (size_t i = 0; i <= n-1; ++i) {
            for (size_t j = 0; j <= n-1-i; ++j) {
                size_t k = n-1-i-j;              /* i + j + k = n-1 */

                /* first convolve tri[i] and tri[j] into conv[] */
                size_t lenAB = i + j + 1;        /* maximum index in conv */

                /* zero conv up to lenAB-1 */
                for (size_t t = 0; t < lenAB; ++t) conv[t] = 0;

                for (size_t a = 0; a <= i; ++a)
                    for (size_t b = 0; b <= j; ++b)
                        conv[a+b] += tri[i][a] * tri[j][b];

                /* convolve conv[] with tri[k] and add to row n shifting by 1 */
                for (size_t a = 0; a < lenAB; ++a) {
                    seq_t cv = conv[a];
                    if (cv == 0) continue;
                    for (size_t c = 0; c <= k; ++c) {
                        tri[n][a + c + 1] += cv * tri[k][c];
                    }
                }
                /* conv buffer will be overwritten in next iteration */
            }
        }
    }

    cur_rows = new_rows;
}

/* ----------  convert flat index to (row,col)  ---------- */
static void index_to_rowcol(idx_t n, idx_t *row, idx_t *col)
{
    /* Solve r(r+1)/2 <= n < (r+1)(r+2)/2 */
    long double d = sqrt((long double)(8.0L * n + 1.0L));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    idx_t first = r * (r + 1) / 2;
    *row = r;
    *col = n - first;
}

/* ----------  the required function  ---------- */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rowcol(n, &r, &c);
    ensure_rows(r);
    return tri[r][c];
}
