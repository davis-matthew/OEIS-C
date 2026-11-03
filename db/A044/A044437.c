/*
 * A044437  Numbers n such that string 1,1 occurs in the base 3 representation
 *          of n but not of n+1.
 * https://oeis.org/A044437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the string "11" occurs in the base‑3 representation of n but not of n+1 */
#include <stdbool.h>

static inline bool has_11(seq_t x) {
    seq_t prev = 3; /* a value outside 0,1,2 */
    while (x > 0) {
        seq_t d = x % 3;
        if (d == 1 && prev == 1) return true;
        prev = d;
        x = x / 3;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t candidate = 1;
    while (found < n) {
        if (has_11(candidate) && !has_11(candidate + 1)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
    return 0; /* unreachable for n >= 1 */
}
