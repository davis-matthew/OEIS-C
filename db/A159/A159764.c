/*
 * A159764  Riordan array (1/(1+4x+x^2), x/(1+4x+x^2)).
 * https://oeis.org/A159764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 121, i64 n <= 440,
 *            i128 n <= 1719, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A159764 – flattened Riordan array (1/(1+4x+x^2), x/(1+4x+x^2))
   Recurrence for the triangle:
       T(n,k) = (-4)*T(n-1,k) + T(n-1,k-1) - T(n-2,k)
   with   T(0,0) = 1,   T(n,k) = 0 for k>n.
   The sequence a(n) is the linearization of the triangle
   (row by row, left‑to‑right).  a(0)=1, a(1)=-4, a(2)=1, … */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Compute the n-th element of the flattened Riordan array. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column position inside that row. */
    idx_t k = n - (r * (r + 1)) / 2;

    seq_t *prev2 = NULL;   /* row i-2  */
    seq_t *prev1 = NULL;   /* row i-1  */
    seq_t *curr  = NULL;   /* current row */

    for (idx_t i = 0; i <= r; ++i) {
        curr = alloc_seq_array((size_t)(i + 1));
        if (!curr)
            return (seq_t)0;          /* allocation failure – return 0 */

        if (i == 0) {
            curr[0] = (seq_t)1;
        } else {
            for (idx_t j = 0; j <= i; ++j) {
                seq_t t_n1k   = 0;
                seq_t t_n1km1 = 0;
                seq_t t_n2k   = 0;

                if (prev1 && j <= i - 1)   t_n1k   = prev1[j];
                if (prev1 && j > 0)        t_n1km1 = prev1[j - 1];
                if (prev2 && j <= i - 2)   t_n2k   = prev2[j];

                curr[j] = (-4) * t_n1k + t_n1km1 - t_n2k;
            }
        }

        /* rotate rows: prev2 <- prev1; prev1 <- curr; free old prev2 */
        seq_t *old_prev2 = prev2;
        prev2 = prev1;
        prev1 = curr;
        if (old_prev2)
            free_seq_array(old_prev2);
    }

    /* prev1 holds the desired row r. */
    seq_t result = prev1[k];

    /* shut down */ 
    free_seq_array(prev1);
    if (prev2)
        free_seq_array(prev2);

    return result;
}
