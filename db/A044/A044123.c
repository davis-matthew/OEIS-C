/*
 * A044123  Numbers n such that string 3,3 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑6 representation contains
 *            the substring "33" but whose predecessor does not
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has33(seq_t x) {
    bool prev3 = false;
    while (x > 0) {
        seq_t d = x % 6;
        if (prev3 && d == 3) return true;
        prev3 = (d == 3);
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – safe for n==0 */
    seq_t count = 0;
    seq_t i = 0;
    while (1) {
        ++i;
        if (has33(i) && !has33(i - 1)) {
            ++count;
            if (count == n) return i;
        }
    }
}
