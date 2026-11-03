/*
 * A039102  Numbers whose base-9 representation has the same number of 4's and
 *          5's.
 * https://oeis.org/A039102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9731 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute the nth integer (1‑indexed) whose base‑9 representation contains an equal number of 4’s and 5’s */
#include <stdbool.h>

/* Test whether a number has the same count of 4 and 5 digits in base‑9. */
static bool has_equal45(seq_t val) {
    if (val == 0) return true;   /* 0 has zero 4's and zero 5's */
    int c4 = 0, c5 = 0;
    seq_t x = val;
    while (x) {
        int d = (int)(x % 9);
        if (d == 4) c4++;
        else if (d == 5) c5++;
        x /= 9;
    }
    return c4 == c5;
}

/* Return the nth (n ≥ 1) term of OEIS A039102. */
seq_t a(idx_t n) {
    idx_t count = 0;   /* how many valid numbers have been found so far */
    seq_t num = 0;     /* current integer being tested */

    while (1) {
        if (has_equal45(num)) {
            count++;
            if (count == n) return num;
        }
        num++;          /* move to the next integer */
    }
}
