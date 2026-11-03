/*
 * A044545  Numbers n such that string 3,5 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑7 representation contains the
 *            adjacent digit pair 3,5 while that of n+1 does not
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 991 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_35(seq_t v)
{
    int prev = -1;
    while (v > 0) {
        int d = (int)(v % 7);          /* least‑significant digit in base 7 */
        if (d == 3 && prev == 5)       /* pattern 3 5 seen in the correct order */
            return true;
        prev = d;
        v /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* out of bounds – sequence is 1‑indexed */

    idx_t found = 0;

    for (idx_t i = 1; ; ++i) {
        seq_t x  = (seq_t)i;           /* candidate number */
        seq_t xp = x + (seq_t)1;       /* its successor */

        if (has_35(x) && !has_35(xp)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
