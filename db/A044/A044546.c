/*
 * A044546  Numbers n such that string 3,6 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   contains36
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A044546
    Numbers n such that the base‑7 representation of n contains the
    substring "36" but n+1 does not.  This function returns the nth term
    (n ≥ 1).  The algorithm scans integers in increasing order and
    counts those that satisfy the condition.  The search stops when the
    desired index is reached.  The function works with any seq_t
    (uint8_t … unsigned __int128, arbitrary‑precision) and any idx_t
    (uint64_t or equivalent). */
#include <stdbool.h>
#include "../utils/contains36.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined, but protect against overflow */
        return 0;

    idx_t count = 0;
    idx_t candidate = 0;  /* will become 1 on first iteration */

    for (;;) {
        ++candidate;                 /* next integer to test */

        if (contains36((seq_t)candidate)) {
            if (!contains36((seq_t)(candidate + 1))) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
    }
}
