/*
 * A043228  Numbers k such that 1 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that k contains the digit pair 15
 *            or 51 and k‑1 does not contain that pair
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check if the decimal representation of x contains the adjacent digits
   15 or 51 (in that order or reversed). */
static inline bool has_adjacent15or51(seq_t x) {
    if (x < 10)
        return false;                    /* single digit cannot contain the pair */

    seq_t t = x;
    int last = (int)(t % 10);
    t /= 10;

    while (t > 0) {
        int d = (int)(t % 10);
        if ((d == 1 && last == 5) || (d == 5 && last == 1))
            return true;
        last = d;
        t /= 10;
    }
    return false;
}

/* Main function required by the testing harness. */
seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t i = 1;                          /* start searching from 1 */

    while (1) {
        if (has_adjacent15or51((seq_t)i)) {
            if (!has_adjacent15or51((seq_t)(i - 1))) {
                ++count;
                if (count == n)
                    return (seq_t)i;
            }
        }
        ++i;
    }
}
