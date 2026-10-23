/*
 * A043955  Numbers k such that 4 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_four_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computing OEIS sequence A043955.
   Numbers k whose base‑8 representation contains the adjacent digits
   4 and 6 (in either order) while k+1 does not.  The implementation
   simply scans integers in increasing order until the nth qualifying
   number is found.  This is efficient for n up to about 10 000. */

#include <stdbool.h>
#include "../utils/has_adjacent_four_six.h"

/* a(n) = the nth number k (n ≥ 1) such that k has "46" or "64" in base‑8
   but k+1 does not.  The offset is 1; for n ≤ 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* safety for out of range n */
    seq_t k = 1;
    idx_t found = 0;
    while (found < n) {
        if (has_adjacent_four_six(k) && !has_adjacent_four_six(k + 1)) {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
    return 0;                               /* unreachable – kept for completeness */
}
