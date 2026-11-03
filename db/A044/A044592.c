/*
 * A044592  Numbers n such that string 3,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044592: numbers n such that the base‑8 representation of n contains
   the substring "30" but the base‑8 representation of n+1 does not.
*/

#include <stdbool.h>

static bool has30_oct(seq_t x)
{
    if (x == 0) return false;
    seq_t tmp = x;
    int prev = -1;                     /* previous digit (less significant)      */
    while (tmp > 0) {
        int d = (int)(tmp % 8);         /* current digit (0‑7)                     */
        if (prev == 0 && d == 3)        /* 03 in reversed order => 30 in normal   */
            return true;
        prev = d;
        tmp /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t count = 0;      /* how many terms found so far */
    seq_t candidate = 0;  /* current number to test      */
    seq_t last_term = 0;  /* the nth term to return      */

    while (count < n) {
        candidate++;                 /* move to next integer          */
        if (has30_oct(candidate) && !has30_oct(candidate + 1)) {
            last_term = candidate;
            count++;
        }
    }
    return last_term;
}
