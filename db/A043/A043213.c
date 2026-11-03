/*
 * A043213  Numbers k such that 6 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the search for the n‑th number k
 * whose base‑9 representation contains the adjacent digits
 * 6 and 7 (in either order) while the representation of k‑1
 * does not.  It iterates k in increasing order, counting
 * qualifying values until the desired index n is reached. */
#include <stdbool.h>

static inline int has_adjacent_67or76(seq_t x)
{
    if (x == 0) return 0;
    seq_t last = x % 9;
    x /= 9;
    while (x != 0) {
        seq_t d = x % 9;
        if ((last == 6 && d == 7) || (last == 7 && d == 6))
            return 1;
        last = d;
        x /= 9;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 1; ; ++k) {
        if (has_adjacent_67or76(k)) {
            if (!has_adjacent_67or76(k - 1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
    }
}
