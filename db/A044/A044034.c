/*
 * A044034  Numbers k such that 5 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adj_5_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adj_5_9.h"

/* Definition: numbers k such that 5 and 9 occur juxtaposed in base‑10
   representation of k but not of k+1.  The function performs a linear
   search for the nth such number. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n==0, return 0 as default */

    seq_t k = 1;                          /* candidate integer */
    idx_t count = 0;                      /* how many valid k's seen so far */

    for (;;) {                           /* loop until the nth term is found */
        if (has_adj_5_9(k) && !has_adj_5_9(k + 1)) {
            count++;
            if (count == n)
                return k;
        }
        k = k + 1;                        /* advance to next integer */
    }
}
