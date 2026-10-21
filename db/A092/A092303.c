/*
 * A092303  Triangle read by rows giving coefficients of polynomials f_n(q)
 *          arising in several different contexts.
 * https://oeis.org/A092303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, nice
 * Formula:   a(n) is defined by the coefficient‐triangle of the polynomials
 *            f_n(q). For each n the polynomial f_n(q) is decomposed as f_n(q)
 *            = Σ_{r≥0} f_{n,r}(q), where the f_{n,r} satisfy f_{0,0}(q) = 1,
 *            f_{n+1,0}(q) = 1, f_{n+1,r+1}(q) = q^{r+1}\,f_{n,r+1}(q) +
 *            (q^{\,n-r} - q^r)\,f_{n,r}(q). The sequence a(...
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Row data: for row r we keep poly[0 … r], each an array of coeffs up
   to exponent rows[r].len‑1.                                    */
typedef struct {
    seq_t **poly;     /* poly[0 … r] */
    idx_t  len;       /* number of exponents in each poly array         */
} Row;

static Row *rows        = NULL;   /* array of computed rows                */
static idx_t rows_count = 0;      /* number of rows stored                */

/* Ensure that rows[0 … target] are computed. */
static void ensure_rows(idx_t target)
{
    while (rows_count <= target) {
        idx_t r = rows_count;          /* index of new row              */

        /* length of the new row */
        idx_t len = 1;
        if (r >= 1) {
            /* len(r) = len(r‑1) + floor(r/2)          */
            len = rows[r-1].len + (r / 2);
        }

        /* allocate a new Row structure */
        Row newrow;
        newrow.len  = len;
        newrow.poly = (seq_t**)malloc((r + 1) * sizeof(seq_t*));
        if (newrow.poly == NULL) return; /* allocation failure – caller will error */

        /* allocate coefficient arrays for each rank */
        for (idx_t rank = 0; rank <= r; ++rank) {
            newrow.poly[rank] = (seq_t*)calloc((size_t)len, sizeof(seq_t));
            if (newrow.poly[rank] == NULL) return;
        }

        /* initialise the new row according to the recurrence */
        if (r == 0) {
            /* f_0,0 = 1                                           */
            newrow.poly[0][0] = 1;
        } else {
            /* f_{n+1,0} = 1                                         */
            newrow.poly[0][0] = 1;

            /* compute polynomials of ranks 1 … r                  */
            idx_t oldN = r - 1;     /* index of the previous row          */
            for (idx_t R = 1; R <= r; ++R) {
                seq_t *dst  = newrow.poly[R];
                seq_t *src1 = (R <= oldN) ? rows[oldN].poly[R] : NULL;
                seq_t *src2 = rows[oldN].poly[R - 1];

                /* term1 : q^R * f_{oldN,R}  (if src1 exists)      */
                if (src1 != NULL) {
                    for (idx_t i = 0; i < rows[oldN].len; ++i) {
                        if (src1[i] == 0) continue;
                        idx_t j = i + R;
                        if (j < len) dst[j] += src1[i];
                    }
                }

                /* term2 : (q^{oldN-(R-1)} - q^{R-1}) * f_{oldN,R-1} */
                idx_t shift_a = oldN - (R - 1); /* exponent for first part   */
                idx_t shift_b = (R - 1);        /* exponent for second part  */
                for (idx_t i = 0; i < rows[oldN].len; ++i) {
                    if (src2[i] == 0) continue;
                    idx_t j1 = i + shift_a;
                    if (j1 < len) dst[j1] += src2[i];
                    idx_t j2 = i + shift_b;
                    if (j2 < len) dst[j2] -= src2[i];
                }
            }
        }

        /* append the new row to the global array */
        Row *tmp = (Row*)realloc(rows, (rows_count + 1) * sizeof(Row));
        if (tmp == NULL) return;  /* allocation failure */
        rows = tmp;
        rows[rows_count] = newrow;
        rows_count++;
    }
}

/* Return the n‑th term of sequence A092303. */
seq_t a(idx_t n)
{
    idx_t idx  = n;
    idx_t cum  = 0;      /* total number of coefficients processed so far */
    idx_t r    = 0;      /* current row index                               */

    /* locate the row that contains the requested index */
    while (1) {
        ensure_rows(r);
        if (cum + rows[r].len > idx) break;
        cum += rows[r].len;
        ++r;
    }

    idx_t pos = idx - cum;    /* offset inside the found row */

    /* sum the coefficient over all ranks of that row */
    seq_t res = 0;
    for (idx_t R = 0; R <= r; ++R) {
        res += rows[r].poly[R][pos];
    }
    return res;
}
