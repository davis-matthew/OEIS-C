/*
 * A044201  Numbers n such that string 1,6 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth integer n such that the base‑8 representation of n contains
   the substring "16" but the base‑8 representation of (n-1) does not. */
#include <stdbool.h>

static bool has16(seq_t x)
{
    int prev_digit = -1;          /* digit to the right (less significant) */
    while (x > 0) {
        int d = (int)(x % 8);      /* current digit (more significant)      */
        if (prev_digit == 6 && d == 1)
            return true;           /* found "...16" in normal order        */
        prev_digit = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;        /* how many numbers found so far          */
    seq_t candidate = 1;    /* start checking from 1                  */

    while (1) {
        if (has16(candidate) && !has16(candidate - 1)) {
            ++found;
            if (found == n)
                return candidate; /* nth term found                    */
        }
        ++candidate;          /* move to the next integer             */
    }
}
