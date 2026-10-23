/*
 * A044651  Numbers n such that string 2,1 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has21_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044651:
 *  Numbers n such that the base‑9 representation of n contains the
 *  substring "21" but the base‑9 representation of n+1 does not.
 *  Computed by a simple linear scan.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has21_in_base9.h"

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset is 1; behaviour for n<1 is irrelevant */

    idx_t  found = 0;              /* number of terms found so far            */
    seq_t  cand  = (seq_t)1;       /* current integer to test                 */

    for (;; ++cand)
    {
        if (has21_in_base9(cand) && !has21_in_base9(cand + (seq_t)1))
        {
            ++found;
            if (found == n)
                return cand;      /* nth term found                          */
        }
    }
}
