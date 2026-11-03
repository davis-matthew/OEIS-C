/*
 * A050749  Squares containing no pair of consecutive equal digits.
 * https://oeis.org/A050749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = square of the nth integer whose square contains no pair
 *            of consecutive equal digits
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;
    while (true) {
        seq_t sq = x * x;
        if (no_consecutive_equal_digits(sq)) {
            ++count;
            if (count == n) {
                return sq;
            }
        }
        ++x;
    }
}
