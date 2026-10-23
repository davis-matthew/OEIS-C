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
 * Helpers:   has10_in_base3
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
#include "../utils/has10_in_base3.h"

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
