/*
 * A044545  Numbers n such that string 3,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑7 representation contains the
 *            adjacent digit pair 3,5 while that of n+1 does not
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_35
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_35.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* out of bounds – sequence is 1‑indexed */

    idx_t found = 0;

    for (idx_t i = 1; ; ++i) {
        seq_t x  = (seq_t)i;           /* candidate number */
        seq_t xp = x + (seq_t)1;       /* its successor */

        if (has_35(x) && !has_35(xp)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
