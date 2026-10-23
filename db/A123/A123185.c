/*
 * A123185  Triangular array from a zero coefficient sum of recursive
 *          polynomials: p(k, x) = x*p(k - 1, x) + p(k - 2, x).
 * https://oeis.org/A123185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 94, i16 n <= 333, i32 n <= 1242, i64 n <= 4600,
 *            i128 n <= 9020, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123185: coefficients of the polynomials
   p(0,x)=1,
   p(1,x)=x-1,
   p(2,x)=x^2-3x+2,
   with recurrence p(k,x)=x*p(k-1,x)+p(k-2,x) for k≥3.
   The terms are obtained by listing the coefficients of each
   polynomial row by row: row k contains the coefficients
   of p(k,x) for x^0 … x^k.  */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Determine row r and column c (0‑based) in the triangular array. */
    long double d = sqrt(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;          /* 0 ≤ c ≤ r */

    /* Handle the first three rows explicitly. */
    if (r == 0) return 1;
    if (r == 1) return (c == 0) ? (seq_t)-1 : (seq_t)1;
    if (r == 2) return (c == 0) ? (seq_t)2 : (c == 1) ? (seq_t)-3 : (seq_t)1;

    /* Allocate three working arrays of length r+1 (indices 0..r). */
    size_t sz = (size_t)(r + 1);
    seq_t *prev_prev = (seq_t *)calloc(sz, sizeof(seq_t));  /* p(k-2) */
    seq_t *prev      = (seq_t *)calloc(sz, sizeof(seq_t));  /* p(k-1) */
    seq_t *curr      = (seq_t *)calloc(sz, sizeof(seq_t));  /* p(k)   */
    if (!prev_prev || !prev || !curr) { /* allocation failure */
        free(prev_prev); free(prev); free(curr);
        return 0;
    }

    /* Initialize with the known polynomials p(1) and p(2). */
    prev_prev[0] = (seq_t)-1;    prev_prev[1] = (seq_t)1;      /* p(1) */
    prev[0] = (seq_t)2;          prev[1] = (seq_t)-3; prev[2] = (seq_t)1; /* p(2) */

    /* Compute up to row r using the recurrence. */
    for (idx_t k = 3; k <= r; ++k) {
        curr[0] = prev_prev[0];                            /* coeff 0 */
        for (idx_t i = 1; i < k; ++i) {
            seq_t t1 = prev[i - 1];                       /* p(k-1)[i-1] */
            seq_t t2 = (i <= k - 2) ? prev_prev[i] : (seq_t)0; /* p(k-2)[i] */
            curr[i] = t1 + t2;
        }
        curr[k] = prev[k - 1];                            /* coeff k */

        /* Rotate pointers for next iteration. */
        seq_t *tmp = prev_prev;
        prev_prev = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c];

    free(prev_prev);
    free(prev);
    free(curr);
    return result;
}
