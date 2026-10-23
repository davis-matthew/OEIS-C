/*
 * A043109  Numbers k such that 0 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adj_04
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043109:  numbers k such that 0 and 4 occur juxtaposed in the base‑5 representation of k,
    but not in that of k‑1.
    The function counts such numbers starting from k = 1 and returns the n‑th one.
*/

#include <stdbool.h>
#include "../utils/has_adj_04.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;
    bool prevAdj = false;    /* adjacency for k‑1 (initially for 0, which is false) */

    while (1) {
        bool currAdj = has_adj_04(k);
        if (currAdj && !prevAdj) {
            ++count;
            if (count == n)
                return k;
        }
        prevAdj = currAdj;
        ++k;
    }
}
