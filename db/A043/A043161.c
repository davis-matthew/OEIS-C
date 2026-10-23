/*
 * A043161  Numbers k such that 1 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   contains14or41
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th integer k whose base‑8 representation
   contains the adjacent pair of digits 1 and 4 (in either order),
   but (k‑1) does not contain such a pair.  Computed by a simple
   search.  No formula is known. */

#include <stdbool.h>
#include "../utils/contains14or41.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n = 0 */

    seq_t k      = 1;                /* integer being tested */
    idx_t cnt    = 0;                /* number of suitable values found */
    bool prevHas = false;            /* whether (k‑1) has the pattern */

    while (true) {
        bool curHas = contains14or41(k);
        if (curHas && !prevHas) {
            ++cnt;
            if (cnt == n)
                return k;
        }
        prevHas = curHas;
        ++k;
    }
}
