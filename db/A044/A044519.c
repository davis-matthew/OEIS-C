/*
 * A044519  Numbers n such that string 0,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 866 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate the n‑th number whose base‑7 representation contains
   the substring "00" but the representation of n+1 does not. */
#include <stdbool.h>

/* Test whether a base‑7 number has two consecutive zeroes. */
static bool has_consecutive_zeroes(seq_t v)
{
    /* Need at least two digits to have “00”. */
    if (v < 7) return false;
    seq_t prev = v % 7;
    v /= 7;
    while (v > 0) {
        seq_t cur = v % 7;
        if (prev == 0 && cur == 0) return true;
        prev = cur;
        v /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 1;          /* candidate number */
    while (1) {
        if (has_consecutive_zeroes(x) && !has_consecutive_zeroes(x + 1)) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
