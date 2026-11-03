/*
 * A043152  Numbers k such that 0 and 1 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Enumerates numbers k such that in base‑8 the digits 0 and 1 *
 *            appear as adjacent digits (order insensitive) in k but not in
 *            k‑1
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a number contains adjacent digits 0 and 1 in its
 * octal (base‑8) representation.  Order of the digits does not
 * matter; any occurrence of 0 next to 1 suffices. */
static bool has_adjacent_oct(seq_t x)
{
    unsigned prev = 7;           /* sentinel; cannot be 0 or 1 */
    while (x > 0) {
        unsigned d = (unsigned)(x & 7);   /* least‑significant octal digit */
        if ((prev == 0 && d == 1) || (prev == 1 && d == 0))
            return true;
        prev = d;
        x = x >> 3;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t k = 0;
    idx_t count = 0;

    while (count < n) {
        k++;                              
        if (has_adjacent_oct(k) &&
            !has_adjacent_oct(k - 1)) {
            count++;
            if (count == n)
                return k;
        }
    }
    /* unreachable for n >= 1, but required for completeness */
    return 0;
}
