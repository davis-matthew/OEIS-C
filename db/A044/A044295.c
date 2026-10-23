/*
 * A044295  Numbers n such that string 4,8 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_48
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044295 – numbers n whose base‑9 representation contains the
    substring "48" while (n‑1) does not.
    The function returns the n‑th term (a(1) = 44). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_48.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1 – safety clause     */
        return (seq_t)0;

    idx_t  found = 0;           /* how many qualifying numbers seen      */
    seq_t  cand   = 1;          /* current candidate to test             */

    while (found < n)
    {
        if (has_48(cand) && !has_48(cand - (seq_t)1))
        {
            found++;
            if (found == n)
                return cand;
        }
        cand++;
    }

    /* never reached */
    return (seq_t)0;
}
