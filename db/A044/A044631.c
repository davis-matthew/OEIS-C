/*
 * A044631  Numbers k such that string 7,7 occurs in the base 8 representation
 *          of k but not of k+1.
 * https://oeis.org/A044631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 994, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has77
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044631: Numbers k whose base‑8 representation contains the
   substring "77" but the number k+1 does not.  
   This implementation searches integers sequentially. */
#include <stdbool.h>
#include "../utils/has77.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* sequence is defined only for n ≥ 1 */

    idx_t count = 0;
    seq_t k = (seq_t)63;      /* first term listed in the OEIS data */

    for (;;) {
        if (has77(k)) {
            if (!has77(k + 1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
        ++k;
    }
}
