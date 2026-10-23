/*
 * A136533  Coefficients of Laguerre recursive polynomials with an (n+2)!/2
 *          multiplication factor and alpha=a0 =0 from Hochstadt: P(x, n) =
 *          (2*n + a0 + 1 - x)*P(x, n - 1)/(n + 1) - n*P(x, n - 2)/(n + 1);.
 * https://oeis.org/A136533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   Coefficients of Laguerre recursive polynomials with an (n+2)!/2
 *            multiplication factor and alpha=0. a(n) is the coefficient at
 *            position n in the triangle formed by the coefficients of
 *            (n+2)!/2*P(x,n)
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 1
    a(2) = 3, a(3) = –3
    a(4) = 12, a(5) = –24, a(6) = 4
    ...
   The function is memoised for each call individually but
   recomputes up to the requested row. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is 1‑based index in the sequence */
    if (n < 1) return 0; /* out of range, should not happen */

    /* Find the row r and position p (0‑based) */
    idx_t r = 0;
    idx_t cum = 1;           /* cumulative count up to row r */
    while (cum < n) {
        r++;
        cum += r + 1;
    }
    idx_t prev_rows = r * (r + 1) / 2;   /* sum of elements before this row */
    idx_t p = n - prev_rows - 1;        /* position within the row */

    /* Handle first two rows directly */
    if (r == 0) {
        return 1;   /* only one element: 1 */
    }
    if (r == 1) {
        return (p == 0) ? (seq_t)3 : (seq_t)-3;
    }

    /* Allocate three coefficient arrays of size r+1 */
    size_t sz = (size_t)(r + 1);
    seq_t *p0 = (seq_t *)calloc(sz, sizeof(seq_t)); /* Q_{k-2} */
    seq_t *p1 = (seq_t *)calloc(sz, sizeof(seq_t)); /* Q_{k-1} */
    seq_t *p2 = (seq_t *)calloc(sz, sizeof(seq_t)); /* Q_k (temporary) */
    if (!p0 || !p1 || !p2) {
        /* memory allocation failed – return 0 as fallback */
        free(p0); free(p1); free(p2);
        return 0;
    }

    /* Initialise Q_0 and Q_1 */
    p0[0] = 1;                     /* Q_0 */
    p1[0] = 3;  p1[1] = -3;         /* Q_1 */

    seq_t *cur  = p2;  /* will hold Q_k during the loop   */
    seq_t *prev = p1;  /* Q_{k-1}                        */
    seq_t *prev2= p0;  /* Q_{k-2}                        */

    for (idx_t k = 2; k <= r; ++k) {
        size_t deg = (size_t)k;          /* degree of new polynomial */

        /* 1. Clear cur array for indices 0..deg */
        for (size_t i = 0; i <= deg; ++i) {
            cur[i] = 0;
        }

        /* 2. Compute (n+2)(2n+1 - x) * Q_{k-1} */
        seq_t const_factor = (seq_t)(k + 2) * (seq_t)(2 * k + 1);
        seq_t x_factor     = (seq_t)(k + 2);

        for (size_t j = 0; j < deg; ++j) { /* prev has degree k-1 */
            cur[j]   += const_factor * prev[j];
            cur[j+1] -= x_factor * prev[j];
        }

        /* 3. Divide by (k+1) */
        seq_t div = (seq_t)(k + 1);
        for (size_t i = 0; i <= deg; ++i) {
            cur[i] /= div;
        }

        /* 4. Subtract k(k+2) * Q_{k-2} */
        seq_t factor2 = (seq_t)k * (seq_t)(k + 2);
        for (size_t j = 0; j < (size_t)(k - 1); ++j) {
            cur[j] -= factor2 * prev2[j];
        }

        /* 5. Rotate the pointers for next iteration */
        seq_t *tmp = prev2;
        prev2 = prev;
        prev  = cur;
        cur   = tmp;
    }

    /* At the end of loop, 'prev' holds Q_r */
    seq_t result = prev[p];

    /* Free allocated arrays */
    free(p0); free(p1); free(p2);

    return result;
}
