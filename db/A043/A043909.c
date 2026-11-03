/*
 * A043909  Numbers k such that 3 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043909: numbers k such that 3 and 5 occur juxtaposed (in either order)
   in the base‑6 representation of k but not of k+1.
   This implementation searches sequentially for the n‑th qualifying value. */

#include <stdbool.h>

/* Test whether x in base 6 contains the adjacent pair 3–5 or 5–3. */
static bool has_35or53(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    unsigned int last = (unsigned int)(x % 6);
    x /= 6;
    while (x != (seq_t)0) {
        unsigned int cur = (unsigned int)(x % 6);
        if ((cur == 3 && last == 5) || (cur == 5 && last == 3))
            return true;
        last = cur;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; values are requested for n >= 1. */
    if (n == (idx_t)0)
        return (seq_t)0;           /* undefined for offset 1 */

    seq_t k = (seq_t)1;
    idx_t found = (idx_t)0;

    for (;;) {
        if (has_35or53(k) && !has_35or53(k + (seq_t)1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
