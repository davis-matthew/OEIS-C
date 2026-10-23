/*
 * A044720  Numbers n such that string 0,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   nth integer whose decimal representation contains the substring
 *            "07" but n+1 does not
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   contains07
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains07.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but guard for safety */

    seq_t count  = 0;
    seq_t current = 0;     /* candidate numbers start from 1 */

    while (count < n)
    {
        current++; /* next candidate */
        if (contains07(current))
        {
            seq_t next = current + 1;
            if (!contains07(next))
            {
                if (++count == n)
                    return current;
            }
        }
    }

    /* Should never reach here */
    return 0;
}
