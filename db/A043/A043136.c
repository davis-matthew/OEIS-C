/*
 * A043136  Numbers k such that 0 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation of A043136: numbers k such that
 * 0 and 6 occur juxtaposed in the base‑7 representation of k
 * but not of k-1. */

#include <stdbool.h>
#include "../utils/has_adjacent_pair.h"

/* Return the n-th term of the sequence (a(1) is the first listed term). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;
    while (count < n) {
        ++candidate;  /* candidate is of type seq_t, so ++ works for all builds */
        if (has_adjacent_pair(candidate) && !has_adjacent_pair(candidate - 1))
            ++count;
    }
    return candidate;
}
