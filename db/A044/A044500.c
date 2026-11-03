/*
 * A044500  Numbers n such that string 2,5 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning natural numbers for the required property:
   an integer m is accepted if its base‑6 representation contains the substring
   “25” (digit 2 followed by digit 5) but its successor m+1 does not.  The
   nth such number is returned. */

#include <stdbool.h>

/* Test whether a number contains the substring 25 in its base‑6 representation. */
static inline bool has25(seq_t x)
{
    if (x < (seq_t)2) return false;
    idx_t prev = (idx_t)-1;
    while (x > (seq_t)0) {
        idx_t d = (idx_t)(x % (seq_t)6);
        if (prev == (idx_t)5 && d == (idx_t)2) return true;
        prev = d;
        x /= (seq_t)6;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for offset 1, but safety */
    idx_t count = 0;
    idx_t current = 1;
    while (1) {
        if (has25((seq_t)current) && !has25((seq_t)(current + 1))) {
            ++count;
            if (count == n) return (seq_t)current;
        }
        ++current;
    }
}
