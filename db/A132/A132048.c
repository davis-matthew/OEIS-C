/*
 * A132048  3*A007318 - A103451 - A000012.
 * https://oeis.org/A132048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A132048: 3*A007318 - A103451 - A000012.
   The triangle T(r,c) (0 ≤ c ≤ r) is defined as:
       T(r,0) = T(r,r) = 1,
       T(r,c) = 3*binom(r,c) - 1   for 0 < c < r.
   The flattened sequence a(n) is the row‑major traversal of this
   triangle.  The function below returns the n‑th element of that
   sequence for any n ≥ 0.
*/

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r and column c corresponding to index n. */
    idx_t r = 0;
    idx_t offset = 0;          /* start index of current row r */

    while (offset <= n)
    {
        idx_t next = offset + r + 1;   /* start index of next row (r+1) */
        if (n < next)
            break;                     /* n belongs to row r */
        offset = next;                 /* move to next row */
        ++r;
    }

    idx_t c = n - offset;  /* column within the row */

    if (c == 0 || c == r)
    {
        /* Border entries are always 1. */
        return 1;
    }
    else
    {
        /* Interior entries: 3*binomial(r,c) - 1. */
        seq_t val = (seq_t)3 * binom(r, c);
        val -= 1;
        return val;
    }
}
