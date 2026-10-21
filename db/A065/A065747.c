/*
 * A065747  Triangle of Gandhi polynomial coefficients.
 * https://oeis.org/A065747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   ceil_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A065747
    Triangle of Gandhi polynomial coefficients.

    Recurrence:
        B(X,1) = X^3
        B(X,k) = X^3 * ( B(X+1,k-1) - B(X,k-1) )   for k>1

    The (i,j)-entry (i>=1, 1<=j<=2i-1) is the coefficient of X^(2+j)
    in B(X,i).  The sequence a(n) is the triangle read row‑wise,
    with a(1)=1.

    For a given index n (n>=1) let
        r = ceil(sqrt(n))      // row number
        s = (r-1)^2            // last index of previous row
        j = n - s              // column number (1‑based)

    Then a(n) = coefficient of X^(2+j) in B(X,r).  The implementation
    builds the triangle iteratively and stores all needed rows in
    memory, so that subsequent calls are fast.
*/
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/ceil_sqrt.h"

/* ---------- Binomial table ------------------------------------- */
static seq_t **binom = NULL;
static idx_t binom_size = 0;          // largest n for which binom[n] is allocated

static void ensure_binom(idx_t n) {
    if (binom_size > n) return;
    idx_t old = binom_size;
    binom_size = n + 1;
    binom = (seq_t**)realloc(binom, sizeof(seq_t*) * binom_size);
    for (idx_t i = old; i < binom_size; ++i) {
        binom[i] = (seq_t*)calloc((size_t)i + 1, sizeof(seq_t));
        if (i > 0) {
            binom[i][0] = (seq_t)1;
            binom[i][i] = (seq_t)1;
            for (idx_t j = 1; j < i; ++j) {
                binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
            }
        } else {
            binom[i][0] = (seq_t)1;
        }
    }
}

/* ---------- Row data ------------------------------------------ */
typedef struct {
    seq_t *entries;       /* flattened triangle values, length = 2*i-1 */
    size_t entries_len;
    seq_t *poly;          /* full polynomial coefficients, index = exponent */
    size_t poly_len;      /* degree + 1 */
} Row;

static Row *rows = NULL;             /* array of computed rows (1‑based) */
static idx_t rows_used = 0;          /* number of rows currently stored */

/* ---------- Compute rows up to r --------------------------------*/
static void compute_rows_up_to(idx_t r) {
    for (idx_t anew = rows_used + 1; anew <= r; ++anew) {
        Row newrow;
        newrow.entries = NULL;
        newrow.entries_len = 0;
        newrow.poly = NULL;
        newrow.poly_len = 0;

        if (anew == 1) {    /* Row 1 */
            newrow.entries_len = 1;
            newrow.entries = (seq_t*)calloc(1, sizeof(seq_t));
            newrow.entries[0] = (seq_t)1;

            newrow.poly_len = 4;          /* degree 3 -> indices 0..3 */
            newrow.poly = (seq_t*)calloc(newrow.poly_len, sizeof(seq_t));
            newrow.poly[3] = (seq_t)1;
        } else {
            /* use previous row */
            Row *prev = &rows[anew - 2];
            idx_t degPrev = (idx_t)prev->poly_len - 1;
            ensure_binom(degPrev);

            /* shifted = P(X+1) */
            seq_t *shifted = (seq_t*)calloc((size_t)degPrev + 1, sizeof(seq_t));
            for (idx_t t = 0; t <= degPrev; ++t) {
                if (prev->poly[t] == (seq_t)0) continue;
                for (idx_t k = 0; k <= t; ++k) {
                    shifted[k] += prev->poly[t] * binom[t][k];
                }
            }

            /* diff = shifted - prev */
            seq_t *diff = (seq_t*)calloc((size_t)degPrev + 1, sizeof(seq_t));
            for (idx_t k = 0; k <= degPrev; ++k) {
                diff[k] = shifted[k] - prev->poly[k];
            }
            free(shifted);

            /* new polynomial = X^3 * diff  */
            idx_t newLen = degPrev + 3;     /* degPrev+2 + 1 */
            seq_t *newPoly = (seq_t*)calloc((size_t)newLen, sizeof(seq_t));
            for (idx_t k = 0; k <= degPrev; ++k) {
                if (k + 3 < newLen) newPoly[k + 3] = diff[k];
            }
            free(diff);

            /* Store new row */
            newrow.entries_len = 2 * anew - 1;
            newrow.entries = (seq_t*)calloc(newrow.entries_len, sizeof(seq_t));
            for (idx_t j = 1; j <= newrow.entries_len; ++j) {
                idx_t exp = 2 + j;   /* exponent of X */
                if (exp < newLen) {
                    newrow.entries[j - 1] = newPoly[exp];
                }
            }

            newrow.poly = newPoly;
            newrow.poly_len = newLen;
        }

        /* Expand rows array if necessary */
        rows = (Row*)realloc(rows, sizeof(Row) * anew);
        rows[anew - 1] = newrow;
        rows_used = anew;
    }
}

/* ---------- Sequence function ---------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                /* offset 1; n>=1 */

    idx_t r = ceil_sqrt(n);
    compute_rows_up_to(r);

    idx_t j = n - (r - 1) * (r - 1);            /* 1‑based column */
    return rows[r - 1].entries[j - 1];
}
