/*
 * A044327  Numbers n such that string 8,4 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has84
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose base‑9 representation contains the
   substring “84” while n − 1 does not.  The sequence is OEIS A044327,
   offset 1 (a(1) = 76). */

#include <stdbool.h>
#include "../utils/has84.h"

/* Returns the n-th value of the sequence (n >= 1).  The function
   increments successive integers until it finds the n‑th one that
   satisfies the definition.  This linear search is fast enough for
   n up to about 10 000, as the density of such numbers is roughly
   1 in 75. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t number = 1;

    for (;; ++number) {
        if (has84(number)) {
            seq_t prev = number - 1;
            if (!has84(prev)) {
                ++count;
                if (count == n)
                    return number;
            }
        }
    }
    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
