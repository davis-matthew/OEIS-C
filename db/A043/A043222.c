/*
 * A043222  Numbers k such that 0 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043222
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

/* Sequence A043222.
   a(n) = the n-th positive integer k such that k has the digits 0 and 7 adjacent
   (i.e., contains the substring "07" or "70") while k‑1 does NOT have such an adjacency. */

#include <stdbool.h>

/* Test whether a number x contains an adjacent pair of digits 0 and 7. */
static bool has_adjacent_07_70(seq_t x)
{
    /* No adjacency possible for a single digit. */
    if (x < 10) return false;

    seq_t prev = x % 10;   /* least significant digit */
    x /= 10;

    while (x > 0) {
        seq_t cur = x % 10;
        if ((cur == 0 && prev == 7) || (cur == 7 && prev == 0))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, but safe guard */

    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        if (has_adjacent_07_70(k) && !has_adjacent_07_70(k - 1)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
