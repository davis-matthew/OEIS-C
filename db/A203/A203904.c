/*
 * A203904  Triangular array T; for n>0, row n shows the coefficients of a
 *          reduced polynomial having zeros -k/(n+1) for k=1,2,...,n.
 * https://oeis.org/A203904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   gcd, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A203904: a(n) is the coefficient in the reduced polynomial
 *   P_m(x) = ∏_{k=1}^m ((m+1)x + k)
 * where the rows are flattened in order of increasing m.
 * The coefficients are reduced by their greatest common divisor.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_alloc.h"

/* Cache of computed rows.  __rows[r] points to the
   array of coefficients for row r (0‑based).  Rows are stored
   in order of increasing r. */
static seq_t **__rows = NULL;       /* array of row pointers   */
static size_t   __rows_count = 0;   /* number of rows cached   */

/* ------------------------------------------------------------------ */
/* Compute the coefficients for a single row n (degree = n).         */
/* ------------------------------------------------------------------ */
static void compute_row(idx_t n)
{
    /* Allocate storage for row n (length n+1). */
    seq_t *row = seq_alloc((size_t)n + 1);
    if (!row) return;  /* allocation failure – unlikely in tests */

    /* Initialise: constant term = 1, others = 0 */
    row[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i)
        row[i] = (seq_t)0;

    /* Multiply by each factor ((n+1)x + k).  Do it in place
       scanning from the back to avoid overwriting. */
    const seq_t coeff_x = (seq_t)(n + (idx_t)1);   /* (n+1)  */

    for (size_t k = 1; k <= (size_t)n; ++k)
    {
        const seq_t coeff_const = (seq_t)k;         /* k */

        /* Update from high degree down to 1 */
        for (size_t m = k; m > 0; --m)
        {
            row[m] = row[m] * coeff_const + row[m-1] * coeff_x;
        }
        /* Constant term */
        row[0] *= coeff_const;

        /* Reduce by gcd of all coefficients immediately
           (optional: can be done after all k, but keeps numbers small). */
        seq_t g = (seq_t)0;
        for (size_t i = 0; i <= k; ++i)
            g = gcd(g, row[i]);

        if (g > (seq_t)1)
        {
            for (size_t i = 0; i <= k; ++i)
                row[i] /= g;
        }
    }

    /* Store the new row in the cache. */
    /* Expand the cache array if necessary. */
    if (__rows == NULL) {
        __rows = (seq_t **)malloc(sizeof(seq_t *) * 1);
    } else {
        __rows = (seq_t **)realloc(__rows, (__rows_count + 1) * sizeof(seq_t *));
    }
    __rows[__rows_count++] = row;
}

/* ------------------------------------------------------------------ */
/* Ensure that rows up to and including 'last' have been computed.    */
/* ------------------------------------------------------------------ */
static void ensure_rows_up_to(idx_t last)
{
    while (__rows_count <= last) {
        compute_row((idx_t)__rows_count);
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;            /* below offset – unspecified */

    /* Convert the 1‑based index to the 0‑based position in the
       flattened array. */
    idx_t pos = n - (idx_t)1;

    /* Find the row r (degree) and the column j within that row. */
    idx_t r = 0;                    /* current row index (0‑based) */
    idx_t rem = pos;                /* remaining offset within triangular array */
    while (rem >= r + (idx_t)1) {
        rem -= r + (idx_t)1;
        ++r;
    }
    idx_t j = rem;                  /* column index within row r (0‑based) */

    /* Make sure the required row has been computed. */
    ensure_rows_up_to(r);

    /* Return the requested coefficient. */
    return __rows[r][j];
}
