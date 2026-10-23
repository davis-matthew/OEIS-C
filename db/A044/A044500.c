/*
 * A044500  Numbers n such that string 2,5 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning natural numbers for the required property:
   an integer m is accepted if its base‑6 representation contains the substring
   “25” (digit 2 followed by digit 5) but its successor m+1 does not.  The
   nth such number is returned. */

#include <stdbool.h>
#include "../utils/has25.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for offset 1, but safety */
    idx_t count = 0;
    idx_t current = 1;
    while (1) {
        if (has25((seq_t)current) && !has25((seq_t)(current + 1))) {
            ++count;
            if (count == n) return (seq_t)current;
        }
        ++current;
    }
}
