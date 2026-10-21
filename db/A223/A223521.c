/*
 * A223521  Triangle T(n,k) represents the coefficients of (x^19*d/dx)^n,
 *          where n=1,2,3,...
 * https://oeis.org/A223521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = the n-th element when the rows of the triangle * T(n,k),
 *            whose k-th entry is the coefficient of D^k in the operator *
 *            (x^19 d/dx)^n, are concatenated row by row
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the requested entry on the fly.
 * It builds successive rows of the triangle using the recurrence:
 *
 *     new_c[0]   = old_c[0] * old_e[0];
 *     new_e[0]   = old_e[0] + 18;          // r-1 with r=19
 *
 *     for k = 1 .. old_len-1:
 *         new_c[k] = old_c[k] * old_e[k] + old_c[k-1];
 *         new_e[k] = old_e[k] + 18;
 *
 *     new_c[old_len] = old_c[old_len-1];
 *     new_e[old_len] = old_e[old_len-1] + 19;
 *
 * where old_c and old_e are the coefficient and exponent arrays of
 * the previous row.  The cumulative number of elements produced so
 * far is maintained until the requested index falls within the
 * current row.  No dynamic caching between calls is used.
 */

#include <stdlib.h>
#include <stdint.h>

#define R 19
/* r_minus_1 is used repeatedly in the inner loop. */
#define R_MINUS_1 (R - 1)

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* We need at most n rows to reach the nth element. */
    idx_t max_rows = n;

    /* Allocate four arrays of size max_rows. */
    seq_t *prev_c = (seq_t *)malloc(max_rows * sizeof(seq_t));
    seq_t *prev_e = (seq_t *)malloc(max_rows * sizeof(seq_t));
    seq_t *next_c = (seq_t *)malloc(max_rows * sizeof(seq_t));
    seq_t *next_e = (seq_t *)malloc(max_rows * sizeof(seq_t));
    if (!prev_c || !prev_e || !next_c || !next_e) {
        /* Allocation failed: handle as zero (unlikely with limits). */
        return (seq_t)0;
    }

    idx_t prev_len = 1;            /* length of the previous row */
    idx_t cumulative = 0;          /* number of elements generated so far */

    /* Initialise the first row: coefficient 1, exponent R. */
    prev_c[0] = (seq_t)1;
    prev_e[0] = (seq_t)R;

    while (1) {
        /* Does the desired term lie in the current row? */
        if (cumulative + prev_len >= n) {
            idx_t idx_in_row = n - cumulative - 1;   /* zero‑based */
            seq_t result = prev_c[idx_in_row];
            /* Free allocated memory */
            free(prev_c);
            free(prev_e);
            free(next_c);
            free(next_e);
            return result;
        }

        /* Prepare next row of length prev_len + 1 */
        idx_t new_len = prev_len + 1;

        /* First element */
        next_c[0] = prev_c[0] * prev_e[0];
        next_e[0] = prev_e[0] + R_MINUS_1;

        /* Middle elements */
        for (idx_t k = 1; k < new_len - 1; ++k) {
            next_c[k] = prev_c[k] * prev_e[k] + prev_c[k-1];
            next_e[k] = prev_e[k] + R_MINUS_1;
        }

        /* Last element */
        next_c[new_len - 1] = prev_c[prev_len - 1];
        next_e[new_len - 1] = prev_e[prev_len - 1] + R;

        /* Advance: current row becomes the previous row */
        cumulative += prev_len;
        prev_len = new_len;

        /* Swap pointers to avoid copying */
        seq_t *tmp = prev_c; prev_c = next_c; next_c = tmp;
        tmp = prev_e;        prev_e = next_e; next_e = tmp;
    }

    /* This point is never reached, but return 0 to silence warnings. */
    return (seq_t)0;
}
