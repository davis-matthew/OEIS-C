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
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine returns the nth number (OEIS offset 1) such that the base‑4
   representation of the number contains the substring "11" while the base‑4
   representation of the number plus one does not.  
   It scans natural numbers until the nth such number is found.  */

#include <stdbool.h>

/* Helper: true if x in base 4 contains adjacent digits 1,1 */
static bool has_sub11(seq_t x)
{
    seq_t prev = 0;          /* digit to the left of current (least significant first) */
    while (x > 0)
    {
        seq_t cur = x % 4;
        if (cur == 1 && prev == 1)
            return true;
        prev = cur;
        x /= 4;
    }
    return false;
}

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
