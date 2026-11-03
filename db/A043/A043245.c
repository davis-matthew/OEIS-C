/*
 * A043245  Numbers k such that 3 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k containing "39" or "93", but not in k-1 */
#include <stdbool.h>

/* Check whether a non‑negative integer x contains the adjacent
   digits 3 and 9 in either order. */
static bool has_39_93(idx_t x)
{
    if (x < 10) return false;          /* a single digit cannot contain a pair */
    int prev = x % 10;
    x /= 10;
    while (x > 0) {
        int cur = x % 10;
        if ((cur == 3 && prev == 9) || (cur == 9 && prev == 3))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    bool prev_has = false;          /* has_39_93(0) == false */

    while (count < n) {
        k++;                         /* start at 1 */
        bool cur_has = has_39_93(k);
        if (cur_has && !prev_has) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        prev_has = cur_has;
    }
    /* Unreachable, but keep the compiler happy */
    return (seq_t)k;
}
