/*
 * A043096  Numbers in which every pair of adjacent digits are distinct.
 * https://oeis.org/A043096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th non‑negative integer whose decimal digits
 *            contain no pair of equal adjacent digits (brute force
 *            enumeration)
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 125 terms through n = 9920 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    seq_t val = 0;          /* a(1) = 0 */
    idx_t count = 1;
    if (n == 1) return val;

    while (true) {
        val = val + 1;      /* next integer */
        if (no_consecutive_equal_digits(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
