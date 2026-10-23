/*
 * A043902  Numbers k such that 1 and 3 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has13_adjacent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A043902:
   a(n) = the n‑th integer k such that
          in the base‑6 representation of k the digits 1 and 3
          appear as adjacent (in either order), and
          the same does not hold for k+1.
   Search is performed by scanning successive integers.  This is
   the straightforward brute‑force method.  It works for any
   seq_t type (including arbitrary precision) and for any n >= 1. */
#include <stdbool.h>
#include "../utils/has13_adjacent.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, harmless value */

    idx_t found = 0;   /* number of qualifying k found so far */
    seq_t k = 0;       /* candidate integer */

    while (true) {
        ++k;                                 /* examine successive integers */
        if (has13_adjacent(k) && !has13_adjacent(k + 1)) {
            ++found;
            if (found == n)
                return k;
        }
    }
}
