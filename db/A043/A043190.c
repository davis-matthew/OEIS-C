/*
 * A043190  Numbers k such that 1 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has14_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate integers k and find the nth number whose base‑9
   representation contains the digits 1 and 4 as an adjacent pair
   (either "14" or "41"), while the representation of k‑1 does not
   contain such a pair.  This implements OEIS A043190 as described. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has14_base9.h"

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* behaviour for n==0 is irrelevant */

    idx_t   found = 0;                        /* number of valid terms found */
    seq_t   k     = (seq_t)1;                 /* current candidate integer */

    while (found < n) {
        if (has14_base9(k) && !has14_base9(k - (seq_t)1)) {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
    return k;                                 /* never reached, keeps compiler happy */
}
