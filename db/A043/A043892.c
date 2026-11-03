/*
 * A043892  Numbers k such that 1 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n‑th integer k whose base‑5 representation contains an
   adjacent pair of digits 1 and 4 (in either order) while k+1 does not. */
#include <stdbool.h>

static inline bool has_adjacent14(seq_t x)
{
    int prev = -1;                       /* no previous digit yet */
    while (x > 0) {
        int cur = (int)(x % 5);          /* least‑significant base‑5 digit */
        x /= 5;                          /* shift right one base‑5 digit */
        if ((prev == 4 && cur == 1) ||
            (prev == 1 && cur == 4))
            return true;                 /* adjacent 1 and 4 found */
        prev = cur;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t count = 0;
    seq_t k = 1;

    while (1) {
        if (has_adjacent14(k) && !has_adjacent14(k + 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
