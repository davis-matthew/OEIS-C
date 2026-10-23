/*
 * A043398  Numbers whose base-7 representation contains exactly two 1's.
 * https://oeis.org/A043398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_two_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑7 representation contains exactly
   two 1's. */
#include <stdbool.h>
#include "../utils/has_two_ones.h"

seq_t a(idx_t n)
{
    idx_t count     = 0;
    idx_t candidate = 0;

    while (count < n)
    {
        candidate++;
        if (has_two_ones(candidate))
            count++;
    }
    return (seq_t)candidate;
}
