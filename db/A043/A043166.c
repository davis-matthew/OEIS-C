/*
 * A043166  Numbers k such that 2 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation of OEIS A043166:
   Numbers k such that the base‑8 representation of k has the digits
   2 and 4 adjacent (either order) and k‑1 does not. */
#include <stdbool.h>

/* Return true if x contains an adjacent pair (2,4) or (4,2) in its octal representation. */
static bool has_adjacent_2_4(seq_t x) {
    seq_t prev = 8;                      /* sentinel outside 0–7 */
    while (x > 0) {
        seq_t cur = x & 7;               /* least significant octal digit */
        if ((prev == 4 && cur == 2) || (prev == 2 && cur == 4))
            return true;
        prev = cur;
        x >>= 3;                        /* divide by 8 */
    }
    return false;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    while (count < n) {
        k++;
        if (has_adjacent_2_4(k) && !has_adjacent_2_4(k - 1))
            count++;
    }
    return k;
}
