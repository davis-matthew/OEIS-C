/*
 * A031459  Numbers whose base-3 representation has 3 fewer 0's than 2's.
 * https://oeis.org/A031459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer whose base‑3 representation
   contains exactly three more 2's than 0's (i.e. count_2 - count_0 == 3). */
/* Computation: brute‑force search over integers, checking the digit
   condition directly. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of qualifying numbers found */
    idx_t x = 0;              /* candidate integer */

    for (;; ++x) {
        /* Count 0's and 2's in base‑3 representation of x */
        idx_t y = x;
        idx_t c0 = 0, c2 = 0;

        if (y == 0) { /* special case for 0: representation "0" */
            c0 = 1;
            c2 = 0;
        } else {
            while (y > 0) {
                unsigned d = y % 3;
                if (d == 0)
                    ++c0;
                else if (d == 2)
                    ++c2;
                y /= 3;
            }
        }

        if (c2 - c0 == (idx_t)3) {
            ++found;
            if (found == n) {
                /* Cast to seq_t; works for all supported seq_t sizes */
                return (seq_t)x;
            }
        }
    }
}
