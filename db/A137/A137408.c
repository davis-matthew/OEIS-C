/*
 * A137408  Triangular sequence from coefficients of a switched even -odd
 *          polynomial recursion: odd:p(x,n)=2*x*p(x, n - 1) - p(x, n - 2);
 *          even:p(x,n)=(1 - 2*x)*p(x, n - 1) - p(x, n - 2);.
 * https://oeis.org/A137408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 32, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the polynomials p(x,n):
   p(x,-1)=0,      p(x,0)=1,      p(x,1)=2*x
   odd n:  p(x,n)=2*x*p(x,n-1)-p(x,n-2)
   even n: p(x,n)=(1-2*x)*p(x,n-1)-p(x,n-2)
   The sequence a(n) is obtained by listing the coefficients of
   p(x,0), p(x,1), p(x,2), … in row‑major order.  */

/* The following function returns the n‑th term (n >= 1) of
   OEIS A137408.  It works for any build where the types
   seq_t (sequence values) and idx_t (indices) are defined. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Outside the defined offset – return 0 for safety. */
        return (seq_t)0;
    }

    /* Cumulative number of terms seen so far. */
    idx_t cumulative = 0;
    /* Current row index (starting at 0). */
    idx_t r = 0;
    /* Pointers to the last two rows.  prev_prev refers to row r-2,
       prev refers to row r-1.  They are NULL when the corresponding
       rows are not yet computed. */
    seq_t *prev_prev = NULL;
    seq_t *prev      = NULL;

    while (1) {
        size_t len = (size_t)(r + 1);                     /* number of coeffs in row r */
        seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cur) {                                       /* allocation failure */
            return (seq_t)0;
        }

        /* Build row r. */
        if (r == 0) {                                      /* p(x,0) = 1 */
            cur[0] = (seq_t)1;
        } else if (r == 1) {                               /* p(x,1) = 2*x */
            cur[0] = (seq_t)0;
            cur[1] = (seq_t)2;
        } else {
            bool is_even = ((r & (idx_t)1) == 0);
            if (is_even) {
                /* Even r: p(x,r) = (1-2*x)*p(x,r-1) - p(x,r-2) */
                for (size_t j = 0; j < len; ++j) {
                    seq_t term1 = (j <= (size_t)(r-1)) ? prev[j] : (seq_t)0;
                    seq_t term2 = (j == 0)                 ? (seq_t)0
                                                               : (prev[j-1] * (seq_t)2);
                    seq_t term3 = (j <= (size_t)(r-2)) ? prev_prev[j] : (seq_t)0;
                    cur[j] = term1 - term2 - term3;
                }
            } else {
                /* Odd r: p(x,r) = 2*x*p(x,r-1) - p(x,r-2) */
                for (size_t j = 0; j < len; ++j) {
                    seq_t term1 = (j == 0) ? (seq_t)0 : (prev[j-1] * (seq_t)2);
                    seq_t term2 = (j <= (size_t)(r-2)) ? prev_prev[j] : (seq_t)0;
                    cur[j] = term1 - term2;
                }
            }
        }

        /* Update cumulative count and check if the requested term
           lies within the current row. */
        cumulative += (idx_t)len;
        if (n <= cumulative) {
            idx_t idx = n - (cumulative - (idx_t)len) - (idx_t)1;
            seq_t result = cur[(size_t)idx];

            /* Clean up and return. */
            free(prev);
            free(prev_prev);
            free(cur);
            return result;
        }

        /* Prepare for the next iteration.  Free the oldest buffer. */
        free(prev_prev);
        prev_prev = prev;
        prev      = cur;
        ++r;
    }
}
