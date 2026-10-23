/*
 * A159879  Numbers n such that digit sum of n^2 is 2*(digit sum of n).
 * https://oeis.org/A159879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 3001, u64 n <= 3001,
 *            u128 n <= 3001, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer that satisfies digit_sum(n^2) == 2*digit_sum(n)
   (OEIS A159879).  The function scans integers starting from 0,
   counting those that meet the property until the requested index is reached. */
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t count = 0;           /* number of qualifying integers found */
    idx_t i = 0;               /* current candidate integer (unsigned) */

    for (;; ++i) {
        seq_t x  = (seq_t)i;          /* cast to seq_t for arithmetic */
        seq_t sq = x * x;             /* compute n^2 (within seq_t range) */
        seq_t ds_x  = digit_sum(x);   /* digit sum of n */
        seq_t ds_sq = digit_sum(sq);  /* digit sum of n^2 */

        if (ds_sq == 2 * ds_x) {
            ++count;
            if (count == n)
                return x;             /* found the n-th element */
        }
    }
}
