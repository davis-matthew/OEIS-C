/*
 * A044479  Numbers n such that string 4,1 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044479 – numbers n with base-5 string “41” in n but not in n+1. */

#include <stdbool.h>

/* Helper: check whether the base‑5 representation of x contains the substring 41. */
static bool has41(seq_t x) {
    /* Numbers with fewer than two digits cannot contain “41”. */
    if (x < 5) return false;

    seq_t lower = x % 5;
    x /= 5;
    while (x > 0) {
        seq_t higher = x % 5;
        if (higher == 4 && lower == 1) return true;
        lower = higher;
        x /= 5;
    }
    return false;
}

/* Main sequence function: return the nth term (n >= 1). */
seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t cur   = 1;
    while (1) {
        if (has41(cur) && !has41(cur + 1)) {
            found++;
            if (found == n) return cur;
        }
        cur++;
    }
}
