/*
 * A044304  Numbers n such that string 5,8 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_58
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044304:
   Numbers n such that the base‑9 representation of n contains the contiguous
   substring “58” but the base‑9 representation of n–1 does not.
   The function is implemented by a simple incremental search. */

#include <stdbool.h>
#include "../utils/has_58.h"

/* Return the n-th term of A044304 (n >= 1). */
seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t count = 0;

    while (count < n) {
        ++candidate;                       /* try next integer            */
        if (has_58(candidate) && !has_58(candidate - 1))
            ++count;
    }
    return candidate;
}
