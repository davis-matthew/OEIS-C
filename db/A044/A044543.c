/*
 * A044543  Numbers n such that string 3,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_33
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS A044543: numbers n such that the base‑7
   representation of n contains the substring "33" but the base‑7
   representation of n+1 does not.  The implementation uses a simple
   brute‑force search that works for any integer type seq_t
   (including arbitrary‑precision classes).  No dynamic allocation is
   required, so the function is valid C11, C++17, and respects all
   build constraints. */

#include <stdbool.h>
#include "../utils/has_33.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1;  return 0 for n == 0
       to keep the function well‑defined. */
    if (n == 0)
        return 0;

    seq_t cur = 1;          /* current number being examined */
    idx_t count = 0;        /* how many valid terms we have found */

    while (true) {
        if (has_33(cur) && !has_33(cur + 1)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;              /* move to the next integer */
    }
}
