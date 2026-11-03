/*
 * A043220  Numbers k such that 0 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043220
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

/* Sequence A043220:
 * a(n) = the n-th integer k where the decimal representation of k
 * contains the adjacent digits 0 and 5 in either order (i.e. "05" or "50")
 * but the decimal representation of k‑1 does not contain such a pair.
 * The function enumerates candidates until the nth term is found.
 */

#include <stdbool.h>

static inline bool pair_0_5(seq_t x) {
    /* return true iff x (decimal) has adjacent 0 and 5 digits */
    int prev = -1;          /* previous digit, -1 means none yet */
    while (x > 0) {
        unsigned long long d = (unsigned long long)(x % 10); /* 0 ≤ d < 10 */
        if (prev != -1) {
            if ((prev == 0 && d == 5) || (prev == 5 && d == 0))
                return true;
        }
        prev = (int)d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 1;            /* candidate integers start from 1 */

    for (;;) {
        if (pair_0_5(k)) {
            seq_t km1 = k - 1;
            if (!pair_0_5(km1)) {
                count++;
                if (count == n)
                    return k;
            }
        }
        k++;                 /* move to next integer */
    }
}
