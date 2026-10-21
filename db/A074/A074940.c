/*
 * A074940  Numbers having at least one 2 in their ternary representation.
 * https://oeis.org/A074940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 217, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_two_in_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074940: numbers having at least one digit 2 in their ternary representation. */
/* The function scans integers in order and counts those whose base‑3 representation
   contains the digit 2. The n‑th such integer is returned.  Complexity is O(n). */

#include <stdbool.h>
#include "../utils/has_two_in_base3.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many qualifying numbers found so far  */
    idx_t m;                  /* candidate integer (fits in idx_t)          */

    for(m = 1; ; ++m)         /* start from 1; sequence offset is 1   */
    {
        if (has_two_in_base3((seq_t)m))   /* check this candidate */
        {
            ++count;
            if (count == n)               /* nth found   */
                return (seq_t)m;          /* return as seq_t            */
        }
    }
}
