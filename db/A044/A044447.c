/*
 * A044447  Numbers n such that string 1,1 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_sub11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine returns the nth number (OEIS offset 1) such that the base‑4
   representation of the number contains the substring "11" while the base‑4
   representation of the number plus one does not.  
   It scans natural numbers until the nth such number is found.  */

#include <stdbool.h>
#include "../utils/has_sub11.h"

/* Public function that implements A044447 */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; for n=0 return 0 as a safe placeholder */
        return 0;

    idx_t   found = 0;
    seq_t   num   = 0;

    while (found < n)
    {
        num++;                                 /* candidate number */
        if (has_sub11(num) && !has_sub11(num + 1))
        {
            found++;
            if (found == n)
                return num;
        }
    }
    /* Should never reach here */
    return 0;
}
