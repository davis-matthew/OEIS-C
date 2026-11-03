/*
 * A044083  Numbers k such that string 1,1 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9093 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044083: numbers k such that the base‑5 representation of k
 * contains the substring “11” while the base‑5 representation of k‑1
 * does not.  The function searches for the nth such number. */
#include <stdbool.h>

static bool has_consecutive_ones_base5(idx_t x)
{
    int prev = -1;                 /* previous base‑5 digit, -1 means none yet */
    while (x) {
        int d = (int)(x % 5);       /* current least significant base‑5 digit */
        if (prev == 1 && d == 1)
            return true;            /* found "11" substring */
        prev = d;
        x /= 5;
    }
    return false;                   /* no "11" found */
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but return 0 for safety */
        return (seq_t)0;

    idx_t count = 0;      /* how many valid numbers found so far */
    idx_t k = 1;          /* current candidate */

    for (;; ++k) {
        if (has_consecutive_ones_base5(k) && !has_consecutive_ones_base5(k - 1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
