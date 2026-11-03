/*
 * A032877  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(0)
 *          > d(1) < d(2) > ...
 * https://oeis.org/A032877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A032877: a(n) is the n-th positive integer whose base‑6
 * digits d(0) > d(1) < d(2) > d(3) < … (d(0) is the least‑significant digit).
 * The function scans successive integers, checks the alternating pattern
 * on the fly and stops when the n‑th admissible number is found. */

#include <stdbool.h>

static bool alt6(uint64_t x) {
    /* Return true if the base‑6 digits of x satisfy the alternating
       inequalities: d0 > d1 < d2 > d3 < ... */
    int prev = -1;              /* digit of the lower position */
    bool expect_greater = true; /* first comparison is d0 > d1   */
    while (x) {
        int digit = x % 6;
        if (prev == -1) {       /* first digit (d0) */
            prev = digit;
            x /= 6;
            continue;
        }
        if (expect_greater) {
            if (!(prev > digit)) return false;
        } else {
            if (!(prev < digit)) return false;
        }
        expect_greater = !expect_greater;
        prev = digit;
        x /= 6;
    }
    return true;
}

seq_t a(idx_t n) {
    /* Special case: offset is 1.  n is 1‑based. */
    uint64_t cand = 1;          /* candidate integer (uint64_t is enough up to n~10000) */
    idx_t count = 0;           /* how many admissible numbers found */
    while (1) {
        if (alt6(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
        ++cand;
    }
}
