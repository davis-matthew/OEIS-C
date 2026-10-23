/*
 * A043916  Numbers k such that 0 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_06_60
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of sequence A043916:
   numbers k such that the base‑7 representation of k contains the
   adjacent digits 0 and 6 (in either order) but the representation
   of k+1 does not.  The implementation simply scans the integers
   in increasing order until the required number of terms has been
   found.  It works for any seq_t (unsigned 8/16/32/64/128 bits or an
   arbitrary‑precision integer class) and any idx_t (an unsigned 64‑bit
   integer).  No caching or external libraries are required. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/has_adjacent_06_60.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1; safe default */

    idx_t count = 0;             /* how many valid terms we have seen */
    seq_t k = 1;                 /* candidate integer */

    while (true) {
        if (!has_adjacent_06_60(k)) {
            ++k;
            continue;
        }
        if (has_adjacent_06_60(k + 1)) {
            ++k;
            continue;
        }
        /* k is a valid term */
        ++count;
        if (count == n)
            return k;
        ++k;
    }
}
