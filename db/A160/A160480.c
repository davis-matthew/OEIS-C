/*
 * A160480  The Beta triangle read by rows.
 * https://oeis.org/A160480
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  uned, easy, sign, tabl
 * Formula:   a(n) is the sequence obtained by reading the triangle BETA(n,m)
 *            (n=2,3,… ; m=1,2,…,n-1) by rows, with a(2)=BETA(2,1). The
 *            triangle satisfies BETA(2,1) = -1 BETA(n,1) = (2*n-3)^2 *
 *            BETA(n-1,1) - (2*n-4)! (n≥3) BETA(n,m) = (2*n-3)^2 * BETA(n-1,m)
 *            - BETA(n-1,m-1) (n≥3, 2≤m≤n-2) BETA(n,n-1) = - BETA(...
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 22, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The factorial function is available in the utils library. */
#include <stdlib.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 2, but we simply return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Find the row r and column m that contain a(n). */
    idx_t t = n - 2;          /* zero‑based offset from the first element */
    idx_t r = 2;
    while (t >= r - 1) {
        t -= r - 1;
        ++r;
    }
    idx_t m = t + 1;          /* 1‑based column index within row r */

    /* Build the Beta triangle up to row r. */
    /* Row 2: single element BETA(2,1) = -1. */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = (seq_t)-1;
    size_t prev_len = 1;

    for (idx_t row = 3; row <= r; ++row) {
        size_t curr_len = (size_t)(row - 1);
        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));

        /* Precompute the constant (2*row-3)^2. */
        seq_t coeff = (seq_t)(2 * row - 3);
        coeff = coeff * coeff;

        /* m = 1 case, which needs the factorial term. */
        curr[0] = coeff * prev[0] - factorial(2 * row - 4);

        /* m = 2 .. row-1.  For m = row-1 we use the special rule. */
        for (idx_t mm = 2; mm <= row - 1; ++mm) {
            if (mm <= row - 2) {
                /* Standard recurrence: BETA(n,m) = coeff * BETA(n-1,m) - BETA(n-1,m-1). */
                curr[mm - 1] = coeff * prev[mm - 1] - prev[mm - 2];
            } else { /* mm == row-1 */
                /* BETA(n,n-1) = -BETA(n-1,n-2). */
                curr[mm - 1] = -prev[mm - 2];
            }
        }

        /* Move to the next row. */
        free(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* Extract the requested element. */
    seq_t result = prev[m - 1];
    free(prev);
    return result;
}
