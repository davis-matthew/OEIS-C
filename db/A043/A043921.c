/*
 * A043921  Numbers k such that 1 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043921: numbers k such that the base-7 representation of k
   contains adjacent digits 1 and 6, while the base-7 representation of k+1
   does not. The function returns the nth term (1‑based). */
#include <stdbool.h>
#include "../utils/has_adjacent_16.h"

/* The nth term, n >= 1.  This routine scans successive integers until
   the required count is reached.  The algorithm is O(n * average_gap)
   which is well below the limits for n up to 10000. */
seq_t a(idx_t n)
{
    if (n == 0)        /* behaviour below the offset is irrelevant */
        return 0;
    idx_t   found = 0;  /* number of terms found so far          */
    seq_t   k     = 0;  /* current integer being tested          */

    while (found < n) {
        ++k;
        if (has_adjacent_16(k) && !has_adjacent_16(k + 1)) {
            ++found;
            if (found == n)
                return k;
        }
    }
    /* control never reaches here */
    return 0;
}
