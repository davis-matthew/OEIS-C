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
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer whose decimal representation contains
   the substring "28" but whose successor n+1 does not contain it. */

#include <stdbool.h>

/* Return true if x contains the substring "28".  We scan digits from
   least‑significant to most‑significant, so the pattern 2→8 in the normal
   decimal order is detected as 8 (current digit) followed by 2 (previous
   digit in the loop). */
static bool has28(seq_t x)
{
    int prev = -1;
    while (x > 0)
    {
        int digit = (int)(x % 10);
        if (digit == 2 && prev == 8)
            return true;
        prev = digit;
        x /= 10;
    }
    return false;
}

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
