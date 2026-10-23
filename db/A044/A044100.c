/*
 * A044100  Numbers k such that string 4,3 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044100
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

/* Computed by searching sequentially for the nth number that has the
 * pattern 43 in its base‑5 representation, but whose predecessor does not. */

#include <stdbool.h>

static bool has_43(seq_t x) {
    if (x == 0)
        return false;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 5);
        if (prev == 3 && d == 4)
            return true;
        prev = d;
        x /= 5;
    }
    return false;
}

seq_t a(idx_t n) {
    /* n is 1‑based and always >= 1 */
    seq_t candidate = 1;
    idx_t count = 0;
    while (count < n) {
        if (has_43(candidate) && !has_43(candidate - 1)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;
    }
    return 0; /* unreachable */
}
