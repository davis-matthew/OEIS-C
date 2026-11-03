/*
 * A043192  Numbers k such that 1 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043192
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

/* Algorithm: a(n) returns the n‑th integer k (n ≥ 1) whose base‑9
 * representation contains the adjacent digits 1 and 6 (either order)
 * but the base‑9 representation of k–1 does not contain such a pair.
 * The function searches integers consecutively, counting the
 * qualifying ones until the requested index is reached. */
#include <stdbool.h>

/* Returns true if the base‑9 representation of x contains
 * an adjacent pair 1 and 6 in either order. */
static inline bool has_adjacent_one_six(idx_t x)
{
    int last = x % 9;          /* least significant digit */
    x   /= 9;
    while (x) {
        int cur = x % 9;
        if ((cur == 1 && last == 6) || (cur == 6 && last == 1))
            return true;
        last = cur;
        x   /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t k = 1;           /* candidate integer */
    idx_t found = 0;       /* how many qualifying numbers found */

    while (1) {
        if (has_adjacent_one_six(k) && !has_adjacent_one_six(k - 1)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
