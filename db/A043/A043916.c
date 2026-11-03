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
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
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

/* Return true if x contains the adjacent digits 0 and 6 (in either order)
   in its base‑7 representation. */
static bool has_adjacent_06_60(seq_t x)
{
    if (x == 0)
        return false;
    /* A few dozen digits are enough for the required range. */
    unsigned digits[64];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (unsigned)(tmp % 7);
        tmp /= 7;
    }
    for (size_t i = 0; i + 1 < len; ++i) {
        if ((digits[i] == 6 && digits[i + 1] == 0) ||
            (digits[i] == 0 && digits[i + 1] == 6))
            return true;
    }
    return false;
}

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
