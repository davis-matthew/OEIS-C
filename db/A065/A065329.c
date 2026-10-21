/*
 * A065329  Square array read by antidiagonals giving number of binary trees
 *          of height n with k points on the n-th level (n,k>0).
 * https://oeis.org/A065329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c), where T is defined by * T(1,1) = 1, T(1,k>1) = 0
 *            * T(n,k) = sum_{2j >= k} binom(2j, k) * T(n-1,j) * The sequence
 *            a(n) is read from the array T by antidiagonals: * indices with
 *            n,k > 0 and reading in order of increasing (n + k), * breaking
 *            ties by increasing n. Thus for an index n:...
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 27, u64 n <= 35, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------------------------------------------------------------------- */
/* Parameters (large enough for n <= 10000)                               */
/* ---------------------------------------------------------------------- */
/* The maximum row index needed for n<=10000 is about 141 so 200 is safe. */
#define MAXK    200          /* maximum column index + 1 (0..MAXK)  */
#define MAXROW  200          /* maximum row index + 1 (0..MAXROW)  */

/* ---------------------------------------------------------------------- */
/* Static storage for binomial coefficients and Polynoms (T)              */
/* ---------------------------------------------------------------------- */
static seq_t binom[2*MAXK + 1][MAXK + 1];
static seq_t *polys = NULL;     /* flattened 2D array: [row][col]   */
static size_t rows_computed = 0;
static bool binom_ready = false;

/* ---------------------------------------------------------------------- */
/* Initialize the binomial coefficient table                             */
/* This routine is called lazily on the first request.                   */
/* ---------------------------------------------------------------------- */
static void init_binom(void)
{
    for (size_t n = 0; n <= 2*MAXK; ++n) {
        for (size_t k = 0; k <= MAXK; ++k) {
            binom[n][k] = 0;
        }
    }

    binom[0][0] = 1;
    for (size_t n = 1; n <= 2*MAXK; ++n) {
        binom[n][0] = 1;
        if (n <= MAXK)
            binom[n][n] = 1;

        size_t up = (n < MAXK) ? n : MAXK;
        for (size_t k = 1; k < up; ++k) {
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Ensure that polys is computed up to a given row index <= MAXROW       */
/* ---------------------------------------------------------------------- */
static void ensure_rows(size_t up_to)
{
    if (up_to > MAXROW) up_to = MAXROW;

    if (rows_computed >= up_to) return;

    /* Allocate the polys array once */
    if (!polys) {
        size_t total = (MAXROW + 1) * (MAXK + 1);
        polys = (seq_t *)calloc(total, sizeof(seq_t));
    }

    /* Lazily initialize binomial coefficients */
    if (!binom_ready) {
        init_binom();
        binom_ready = true;
    }

    /* Initialize row 1 (base case) only on the first call */
    if (rows_computed == 0) {
        polys[1 * (MAXK + 1) + 1] = 1;   /* T(1,1) = 1 */
        rows_computed = 1;
    }

    /* Compute missing rows */
    for (size_t n = rows_computed + 1; n <= up_to; ++n) {
        seq_t *prev = &polys[(n-1) * (MAXK + 1)];
        seq_t *cur  = &polys[n * (MAXK + 1)];
        /* zero current row */
        for (size_t k = 0; k <= MAXK; ++k) cur[k] = 0;

        /* recurrence: T(n,k) += T(n-1,j) * binom(2*j,k) for all j */
        for (size_t j = 1; j <= MAXK; ++j) {
            if (prev[j] == 0) continue;
            seq_t *brow = binom[2*j];
            for (size_t k = 0; k <= MAXK; ++k) {
                if (k > 2*j) break;          /* binom[2*j][k] is zero when k>2*j */
                if (brow[k] == 0) continue;
                cur[k] += prev[j] * brow[k];
            }
        }
        rows_computed = n;
    }
}

/* ---------------------------------------------------------------------- */
/* Main sequence function                                                */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Locate antidiagonal: find smallest d with (d-1)*d/2 >= n  */
    idx_t d = (idx_t)((1 + sqrt((long double)(1 + 8*n))) / 2);
    while ((d*(d-1))/2 < n) d++;
    while (d > 0 && ((d-1)*(d-2))/2 >= n) d--;

    idx_t prevM = (d-1)*(d-2)/2;        /* elements before this diagonal */
    idx_t r = n - prevM;                /* row index in the array (1..d-1) */
    idx_t c = d - r;                    /* column index in the array (1..d-1) */

    /* Ensure required rows are computed */
    if (r > MAXROW) return 0;   /* safety: n too large for our bounds */
    ensure_rows((size_t)r);

    seq_t *row = &polys[(size_t)r * (MAXK + 1)];
    return row[(size_t)c];
}
