/*
 * A044262  Numbers n such that string 1,2 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has12_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044262: numbers n whose base‑9 representation contains the
   substring "12" but n‑1 does not.  Computed by brute‑force search. */
#include <stdbool.h>
#include "../utils/has12_base9.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;   /* search starts from 1 */

    while (1) {
        if (has12_base9(candidate) && !has12_base9(candidate - 1)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
