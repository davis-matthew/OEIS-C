/*
 * A044382  Numbers n such that string 5,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains_50
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044382: numbers n such that the decimal representation of
   n contains the substring "50" whereas that of n-1 does not.  */

#include <stdbool.h>
#include "../utils/contains_50.h"

seq_t a(idx_t n)
{
    idx_t   found = 0;          /* how many terms discovered so far   */
    seq_t   curr  = 1;          /* current integer being tested       */
    bool    prev_has = false;   /* did the preceding integer contain "50"? */

    while (true) {
        bool cur_has = contains_50(curr);

        if (cur_has && !prev_has) {           /* a transition to "contains 50" */
            ++found;
            if (found == n)
                return curr;
        }
        prev_has = cur_has;
        ++curr;
    }
}
