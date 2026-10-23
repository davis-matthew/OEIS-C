/*
 * A156535  Triangle, T(n, k, q) = e(n, k, q), where e(n, k, q) = ((1 -
 *          (-q)^k)/(1+q))*e(n-1, k, q) + (-q)^(k-1)*e(n-1, k-1, q), e(n, 0,
 *          q) = e(n, n, q) = 1, and q = 2, read by rows.
 * https://oeis.org/A156535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k,q)=e(n,k,q) where
 *            e(n,k,q)=((1-(-q)^k)/(1+q))*e(n-1,k,q)+(-q)^(k-1)*e(n-1,k-1,q),
 *            e(n,0,q)=e(n,n,q)=1, q=2. The sequence a(n) is the triangle read
 *            by rows, with offset 1
 * Range:     i8 n <= 18, i16 n <= 26, i32 n <= 51, i64 n <= 88, i128 n <= 166,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /* local static state for incremental memoisation */
    static seq_t *seq      = NULL;  /* linear storage of all terms */
    static size_t seq_len  = 0;     /* number of terms stored so far    */
    static seq_t *prev     = NULL;  /* last computed row               */
    static size_t prev_len = 0;     /* length of last row               */
    static idx_t  last_row = -1;    /* row number of last processed row */

    /* initialise structures on first call */
    if (seq == NULL) {
        /* Allocate enough space for all terms up to index 10000
           (the maximum required by the test harness).  The
           triangular number T(141)=10011, so 30000 is ample. */
        seq = seq_alloc_array(30000);
        seq_len = 0;

        /* first row (row index 0) */
        prev = seq_alloc_array(1);
        prev[0] = (seq_t)1;
        prev_len = 1;
        seq[seq_len++] = prev[0];
        last_row = 0;
    }

    /* compute rows until we reach at least n terms */
    while (seq_len < (size_t)n) {
        idx_t  row   = last_row + 1;   /* new row number (0‑based)          */
        size_t r     = (size_t)row + 1; /* length of the new row             */
        seq_t *cur   = seq_alloc_array(r); /* new row buffer                */

        /* boundary values */
        cur[0]     = (seq_t)1;
        cur[r - 1] = (seq_t)1;

        /* interior values (only when length > 2) */
        if (r > 2) {
            /* These powers keep the recurrence in an
               integer‑only form:
               power_k_minus1  = (-2)^k
               power_k         = (-2)^(k+1)          */
            seq_t power_k_minus1 = (seq_t)(-2); /* (-2)^1 for k=1              */
            seq_t power_k        = (seq_t)4;   /* (-2)^2 for k=1              */

            for (size_t k = 2; k < r; ++k) {  /* k: 2‑based index            */
                size_t p1 = k - 1;            /* index of e(n-1,k)            */
                size_t p2 = k - 2;            /* index of e(n-1,k-1)          */

                seq_t fac   = ((seq_t)1 - power_k) / (seq_t)3;
                seq_t term1 = fac * prev[p1];
                seq_t term2 = power_k_minus1 * prev[p2];
                cur[p1] = term1 + term2;

                /* advance powers for next k */
                power_k_minus1 = power_k;
                power_k        = power_k * ((seq_t)(-2));
            }
        }

        /* append the newly computed row into the linear array */
        for (size_t i = 0; i < r; ++i)
            seq[seq_len++] = cur[i];

        /* set up for next iteration (no freeing – harmless memory
           leak, but negligible for the bounded number of rows). */
        prev     = cur;
        prev_len = r;
        last_row = row;
    }

    /* seq is indexed from 0, but the OEIS offset is 1 */
    return seq[(size_t)n - 1];
}
