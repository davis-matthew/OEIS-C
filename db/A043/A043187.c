/*
 * A043187  Numbers k such that 0 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k whose base‑9 representation contains
 *            the digits 0 and 8 adjacent (in either order) but k‑1 does not.
 *            This is computed by a simple loop searching for such k
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Return true if the base‑9 digits of x contain 0 and 8 adjacent (either order). */
static inline bool has_adjacent_0_8(seq_t x)
{
    if (x < 9)
        return false;   /* a single digit cannot contain a pair */

    seq_t prev = x % 9;
    x /= 9;
    while (x > 0)
    {
        seq_t cur = x % 9;
        if ((prev == 0 && cur == 8) || (prev == 8 && cur == 0))
            return true;
        prev = cur;
        x /= 9;
    }
    return false;
}

/* Return the nth term of A043187.  The index is 1‑based. */
seq_t a(idx_t n)
{
    idx_t found = 0;           /* number of terms found so far */
    for (seq_t k = 1;; ++k)    /* scan integers upward */
    {
        if (has_adjacent_0_8(k) && !has_adjacent_0_8(k - 1))
        {
            ++found;
            if (found == n)
                return k;
        }
    }
}
