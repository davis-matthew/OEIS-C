/*
 * A044284  Numbers n such that string 3,6 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_36
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the base‑9 representation of n contains
   the substring "36" but n‑1 does not.  The function searches integers
   sequentially until it has found the n-th such number. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains_36.h"

seq_t a(idx_t n)
{
    seq_t curr = 1;      /* start checking from 1 */
    idx_t found = 0;     /* how many terms found so far */

    while (1) {
        if (contains_36(curr) && !contains_36(curr - 1)) {
            ++found;
            if (found == n)
                return curr;
        }
        ++curr;
    }
}
