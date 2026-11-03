/*
 * A044086  Numbers n such that string 1,4 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044086: n-th integer whose base‑5 representation contains the
   substring "14" while the previous integer does not.  */
#include <stdbool.h>

static bool has_pattern_14_base5(idx_t x)
{
    /* Return true if the base‑5 expansion of x has digits ...1,4 ... */
    idx_t prev = (idx_t)-1;           /* sentinel for “no previous digit” */
    while (x > 0) {
        idx_t d = x % 5;              /* current (least‑significant) digit */
        if (d == 1 && prev == 4)      /* pattern found when scanning LSB→MSB  */
            return true;
        prev = d;
        x /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)                        /* out of OEIS range – undefined   */
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        if (has_pattern_14_base5(m) && !has_pattern_14_base5(m - 1)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
