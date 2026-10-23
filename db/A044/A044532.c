/*
 * A044532  Numbers n such that string 1,6 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   This function searches for the nth integer n such that its base‑7 representation
   contains the adjacent digit pair 1,6 while the base‑7 representation of n+1 does
   not.  It works for all supported seq_t (fixed‑size or arbitrary‑precision) and
   uses only integer operators and comparisons.
*/

#include <stdbool.h>
#include "../utils/has_16.h"

seq_t a(idx_t n)
{
    if (n < 1)                /* the sequence is defined for n≥1          */
        return 0;

    seq_t candidate = (seq_t)1;
    idx_t count = 0;

    for (;;)                   /* scan successive integers              */
    {
        if (has_16(candidate) && !has_16(candidate + (seq_t)1))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}
