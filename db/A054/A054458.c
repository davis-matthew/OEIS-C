/*
 * A054458  Convolution triangle based on A001333(n), n >= 1.
 * https://oeis.org/A054458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the flattened 0‑based row‑wise concatenation of * the
 *            convolution triangle a(n,m) defined for n>=m>=1 by * * a(n,0) =
 *            A001333(n+1) // central trinomial coefficients * a(n,m) =
 *            ((n-m+1)*a(n,m-1) + (2n-m)*a(n-1,m-1) + (n-1)*a(n-2,m-1))/(4*m)
 *            * a(n,m) = 0 if n<m * * The function computes a(n...
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

/* A generous upper bound for row indices up to n = 10 000.
 * The row index r satisfies r(r+1)/2 <= n, thus r < 200 for n <= 10 000.
 * We still use a larger margin for safety. */
#ifndef MAX_ROW
#  define MAX_ROW 200
#endif

seq_t a(idx_t n)
{
    /* Find the row r such that start(r) <= n < start(r+1),
     * where start(r) = r*(r+1)/2 is the index of the first
     * element in row r. */
    idx_t r = 0;
    while (((seq_t)(r + 1) * (r + 2)) / 2 <= n) ++r;

    idx_t start = (idx_t)r * (r + 1) / 2;   /* first index of row r   */
    idx_t k     = n - start;                /* column index within row r */

    /* Static buffers for two previous rows and the current row.  The
       buffers are big enough for the maximum row size we ever need. */
    static seq_t prev2[MAX_ROW + 1];
    static seq_t prev1[MAX_ROW + 1];
    static seq_t cur  [MAX_ROW + 1];

    /* Lengths of the rows stored in the buffers.  They are recomputed
       for each call. */
    size_t len_prev2 = 0, len_prev1 = 0, len_cur = 0;

    /* Initialise the first two base row values for a(n,0) = A001333(n+1) */
    seq_t base0 = 1;   /* a(0,0) */
    seq_t base1 = 3;   /* a(1,0) */

    for (idx_t i = 0; i <= r; ++i) {
        len_cur = (size_t)i + 1;                /* current row length      */
        /* Compute a(i,0) */
        seq_t base;
        if (i == 0) {
            base = base0;
        } else if (i == 1) {
            base = base1;
        } else {
            seq_t new_base = 2 * base1 + base0; /* recurrence for column 0 */
            base0 = base1;
            base1 = new_base;
            base = new_base;
        }
        cur[0] = base;

        /* Compute remaining columns using the 3‑term recurrence */
        for (size_t m = 1; m <= i; ++m) {
            seq_t a_im1  = cur[m - 1];                           /* a(i,m-1)    */
            seq_t a_i1m1 = (m - 1 < len_prev1) ? prev1[m - 1] : 0; /* a(i-1,m-1)  */
            seq_t a_i2m1 = (m - 1 < len_prev2) ? prev2[m - 1] : 0; /* a(i-2,m-1)  */

            seq_t numer = (seq_t)(i - m + 1) * a_im1
                        + (seq_t)(2 * i - m) * a_i1m1
                        + (seq_t)(i - 1)     * a_i2m1;

            cur[m] = numer / ((seq_t)4 * (seq_t)m);
        }

        /* Rotate the row buffers for the next iteration:
         * prev2 <= prev1 <= cur.  Order matters: prev2 gets old prev1. */
        if (i == 0) {
            /* First iteration: only prev1 is set. */
            len_prev1 = len_cur;
            for (size_t t = 0; t < len_cur; ++t) prev1[t] = cur[t];
        } else {
            /* Store old prev1 into prev2. */
            len_prev2 = len_prev1;
            for (size_t t = 0; t < len_prev1; ++t) prev2[t] = prev1[t];

            /* Update prev1 to the current row. */
            len_prev1 = len_cur;
            for (size_t t = 0; t < len_cur; ++t) prev1[t] = cur[t];
        }
    }

    /* After the loop, cur holds row r.  Return the requested column. */
    return cur[k];
}
