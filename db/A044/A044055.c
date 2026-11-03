/*
 * A044055  Numbers n such that string 1,0 occurs in the base 3 representation
 *          of n but not of n-1.
 * https://oeis.org/A044055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 927 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044055 – Numbers n such that the base‑3 string of n contains
   the substring “10” while the base‑3 string of n‑1 does not.  This
   function returns the nth term (offset 1).  The implementation
   scans the base‑3 digits of each candidate and counts the required
   terms.  It works for any build of seq_t (8/16/32/64 bit unsigned
   or arbitrary‑precision).  Complexity is O(n log n).  */

#include <stdbool.h>

/* Return true if the base‑3 representation of x contains the substring
   "10".  We first extract the digits (least significant first) into
   a small array and then check for the pattern 1 followed by 0 in
   the natural string order (from most to least significant).   */
static bool has10_in_base3(seq_t x)
{
    /*  Over 200 digits are plenty for any size that can be produced by
        the sequence up to the limits we test.  */
    int digits[200];
    int len = 0;

    /* Extract digits (least significant first).  */
    do {
        digits[len++] = (int)(x % 3);
        x /= 3;
    } while (x > 0);

    /* Scan for pattern 1 followed by 0 (higher – lower digit).  */
    for (int i = 0; i + 1 < len; ++i) {
        if (digits[i] == 0 && digits[i + 1] == 1)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t cur = 1;      /* current candidate */
    idx_t  cnt = 0;     /* how many valid terms found so far */

    while (cnt < n) {
        /* Move to next integer to inspect. */
        cur++;

        /* Accept if cur has "10" and cur-1 does not.  */
        if (has10_in_base3(cur) && !has10_in_base3(cur - 1))
            ++cnt;
    }
    return cur;
}
