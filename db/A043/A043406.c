/*
 * A043406  Numbers whose base-7 representation contains exactly two 3's.
 * https://oeis.org/A043406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer whose base‑7 representation has exactly
   two digits equal to 3. */

#include <stdbool.h>

/* Counts whether a base‑7 representation of x contains exactly two 3's. */
static bool has_two_threes(seq_t x)
{
    idx_t cnt = 0;
    while (x > 0) {
        if ((x % 7) == 3) {
            cnt++;
            if (cnt > 2)
                return false;
        }
        x /= 7;
    }
    return cnt == 2;
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found = 0;

    /* Scan integers in increasing order until we have found n of them. */
    while (found < n) {
        if (has_two_threes(candidate)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate++;
    }

    /* Unreachable, but keep compiler happy. */
    return (seq_t)0;
}
