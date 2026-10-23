/*
 * A044536  Numbers n such that string 2,3 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_substring_23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044536: numbers n such that the base‑7 representation of n contains
   the substring “23” but the base‑7 representation of n+1 does not. */
#include <stdbool.h>
#include "../utils/has_substring_23.h"

/* Main sequence function.  a(1) is the first such number. */
seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is 1‑based; return 0 for n=0 */
        return 0;

    idx_t count = 0;
    seq_t cand = 1;

    for (;;) {
        if (has_substring_23(cand) && !has_substring_23(cand + 1)) {
            ++count;
            if (count == n)
                return cand;
        }
        ++cand;
    }
}
