/*
 * A043951  Numbers k such that 3 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043951: numbers k such that 3 and 5 occur juxtaposed in
 * the base‑8 representation of k but not in the base‑8 representation
 * of k+1. */
#include <stdbool.h>

static inline bool has_adj_35(seq_t x)
{
    /* Returns true if base‑8 digits of *x* contain 3 and 5 adjacent. */
    int prev = -1;
    while (x) {
        int d = (int)(x % 8);
        if (prev >= 0) {
            if ((d == 3 && prev == 5) || (d == 5 && prev == 3))
                return true;
        }
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t k = 1; ; ++k) {
        if (has_adj_35(k)) {
            seq_t kp = k + 1;
            if (!has_adj_35(kp)) {
                ++found;
                if ((idx_t)found == n)
                    return k;
            }
        }
    }
}
