/*
 * A043917  Numbers k such that 1 and 2 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has12_adjacent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043917: numbers k such that the base‑7 representation of k contains
   the digits 1 and 2 adjacent (in either order) but the base‑7 representation
   of k+1 does not.  The routine brute‑forces this condition, storing the
   found values in a static cache so that arbitrary order of calls is fast. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has12_adjacent.h"

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    /* A static cache of enough terms – 20 000 is more than sufficient
       for the expected range up to n ≈ 10 000. */
    static seq_t cache[20000];
    static idx_t cached_cnt = 0;     /* how many terms stored */
    static idx_t next_k    = 0;      /* next number to test */

    /* Generate terms until we have at least n stored. */
    while (cached_cnt < n) {
        if (has12_adjacent(next_k) && !has12_adjacent(next_k + 1))
            cache[cached_cnt++] = (seq_t)next_k;
        next_k++;
    }

    return cache[n - 1];
}
