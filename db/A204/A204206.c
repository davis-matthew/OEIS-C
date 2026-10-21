/*
 * A204206  Triangle based on (1,3/2,2) averaging array.
 * https://oeis.org/A204206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by enumerating the triangle T(n,k) (1,3/2,2)
 *            averaging array. The recurrence is T(n,1) = 2^n + 1 T(n,2) =
 *            T(n-1,2) + 2^(n-1) + 1, n >= 2, T(2,2)=7 T(n,k) = T(n-1,k) +
 *            3*T(n-1,k-1) - 2*T(n-2,k-1) - 2*T(n-2,k-2), for n >= 3 and k >=
 *            3 The sequence a(n) consists of the entries of th...
 * Range:     u8 n <= 28, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   grow_row, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers, no dependencies on other headers. */
#include <stdlib.h>
#include <stdint.h>

/* Utility function to compute 2^exp as seq_t.  The exact
   implementation is supplied by the build system in the
   ``../utils/pow2.h`` utility.  */
#include "../utils/pow2.h"
#include "../utils/grow_row.h"

/* Public type definitions are provided by the build system:
   seq_t   -- integer type for sequence values
   idx_t   -- unsigned integer type for indexes (uint64_t)         */

/* Dynamic storage for the sequence values.  ``seq`` holds the
   sequence in 0‑based order, i.e. seq[0] = a(1).  */
static seq_t *seq = NULL;
static idx_t seq_cap = 0;   /* number of elements actually allocated      */
static idx_t seq_len = 0;   /* number of elements filled (>= a(i) requested) */

/* Storage for the last two rows of the triangle needed by the
   recurrence.  ``prev`` holds row n-1, ``prevprev`` row n-2.  */
static seq_t *prev = NULL;      /* length = prev_len  */
static idx_t prev_len = 0;
static seq_t *prevprev = NULL;  /* length = prevprev_len  */
static idx_t prevprev_len = 0;

/* Helper: ensure ``seq`` has capacity to hold ``need`` additional
   elements.  The function grows the buffer exponentially.  */
static void ensure_seq_capacity(idx_t need)
{
    if (seq_len + need <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < seq_len + need)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Main function requested by the problem statement.  */
seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 1.  If called with n==0,
       return 0;  the output is undefined for the problem.  */
    if (n == 0)
        return 0;

    /* Generate rows of the triangle until we have at least n
       sequence elements.  */
    while (seq_len < n)
    {
        /* Determine the new row number (1‑based).  */
        idx_t row = prev_len + 1;          /* since prev_len == rows generated so far */
        seq_t *curr = grow_row(row);
        idx_t i;

        if (row == 1)
        {
            /* First row: T(1,1)=3 */
            curr[0] = 3;
        }
        else if (row == 2)
        {
            /* Second row: T(2,1)=5, T(2,2)=7 */
            curr[0] = 5;
            curr[1] = 7;
        }
        else
        {
            /* row >= 3 */

            /* First column: T(n,1) = 2^n + 1 */
            curr[0] = pow2(row) + 1;

            /* Second column: T(n,2) = T(n‑1,2) + 2^(n‑1) + 1 */
            /* prev[1] is the previous row's second column */
            curr[1] = prev[1] + pow2(row - 1) + 1;

            /* Remaining columns (k >= 3) using the general recurrence */
            for (i = 2; i < row; ++i)
            {
                seq_t a_val = (i < prev_len) ? prev[i] : 0;
                seq_t b_val = (i - 1 < prev_len) ? prev[i - 1] : 0;
                seq_t c_val = (i - 1 < prevprev_len) ? prevprev[i - 1] : 0;
                seq_t d_val = (i - 2 < prevprev_len) ? prevprev[i - 2] : 0;

                curr[i] = a_val + 3 * b_val - 2 * c_val - 2 * d_val;
            }
        }

        /* Append the new row's values to the global sequence array.  */
        ensure_seq_capacity(row);
        for (i = 0; i < row; ++i)
            seq[seq_len++] = curr[i];

        /* Shift the row buffers for the next iteration.  */
        free(prevprev);
        prevprev = prev;
        prevprev_len = prev_len;

        prev = curr;
        prev_len = row;
    }

    /* Return the requested term (convert 1‑based to 0‑based).  */
    return seq[n - 1];
}
