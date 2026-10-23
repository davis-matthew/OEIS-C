/*
 * A043954  Numbers k such that 4 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer k such that the base‑8 expansion of k
 *            contains the adjacent digits 4 and 5 (in either order), while
 *            the base‑8 expansion of k+1 does not. The implementation scans
 *            integers in order and counts those satisfying the condition
 *            until the n‑th is found
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x has adjacent digits 4 and 5 in its base‑8 representation
   (either order).  Works for any seq_t type that supports % and / by 8. */
static bool has_adj45(seq_t x)
{
    if (x < 4) return false;          /* impossible to contain 4 and 5 together */

    seq_t prev = 0;                    /* previous (more significant) digit */
    bool first = true;                 /* start of traversal */

    while (x > 0) {
        seq_t digit = x % 8;           /* least‑significant octal digit */

        if (!first) {
            /* Check for either 45 or 54 */
            if ((prev == 4 && digit == 5) ||
                (prev == 5 && digit == 4))
                return true;
        }

        prev   = digit;
        first  = false;
        x      = x / 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* invalid index, defined to be 0 */

    idx_t  count = 0;
    seq_t  k     = 1;                  /* start scanning from 1 */

    while (1) {
        if (has_adj45(k) && !has_adj45(k + (seq_t)1)) {
            ++count;
            if (count == n) return k;
        }
        k = k + (seq_t)1;
    }

    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
