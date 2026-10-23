/*
 * A044741  Numbers n such that string 2,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has28
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer whose decimal representation contains
   the substring "28" but whose successor n+1 does not contain it. */

#include <stdbool.h>
#include "../utils/has28.h"

seq_t a(idx_t n)
{
    /* offset is 1 – indices < 1 are undefined */
    if (n < 1)
        return (seq_t)0;

    seq_t cur = (seq_t)28;          /* first term */
    idx_t count = 1;                /* number of terms found so far */

    if (n == 1)
        return cur;

    for (;;)
    {
        cur = cur + 1;              /* next integer */
        if (has28(cur) && !has28(cur + 1))
        {
            count++;
            if (count == n)
                return cur;
        }
    }
}
