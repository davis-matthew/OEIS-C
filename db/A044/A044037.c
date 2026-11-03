/*
 * A044037  Numbers k such that 6 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(n) = k where k is the n‑th integer whose decimal representation
   contains the substring 69 or 96, but k+1 does not.  Computed by brute‑force
   search. */
#include <stdbool.h>

/* check whether a decimal integer x contains the adjacent digits 6 and 9
   (in either order). */
static bool has_adj_69(seq_t x) {
    seq_t prev = (seq_t)10;          /* sentinel (outside 0–9) */
    while (x > 0) {
        seq_t d = x % 10;
        if (prev != (seq_t)10 &&
            ((prev == 6 && d == 9) || (prev == 9 && d == 6)))
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    /* The OEIS offset is 1, n==0 is undefined – return 0 for safety. */
    if (n == 0)
        return 0;

    seq_t result = 0;
    idx_t found = 0;
    for (seq_t k = 1;; ++k) {
        if (has_adj_69(k) && !has_adj_69(k + 1)) {
            ++found;
            if (found == n) {
                result = k;
                break;
            }
        }
    }
    return result;
}
