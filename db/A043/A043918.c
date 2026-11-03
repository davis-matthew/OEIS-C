/*
 * A043918  Numbers k such that 1 and 3 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by scanning for the nth k such that its base‑7 *
 *            representation contains the digits 1 and 3 adjacent (in either
 *            order), * while the base‑7 representation of k+1 does not
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether the base‑7 representation of x contains an adjacent
 * pair 1‑3 or 3‑1.  Works for both integral and arbitrary‑precision
 * seq_t types. */
static bool has_adjacent13(seq_t x)
{
    bool first = true;
    seq_t last = 0;
    while (x > 0) {
        seq_t d = x % 7;
        if (!first) {
            if ((d == 1 && last == 3) || (d == 3 && last == 1))
                return true;
        }
        first = false;
        last = d;
        x = x / 7;
    }
    return false;
}

/* External-linkage function that returns the nth term (1‑based) of the
 * sequence A043918. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, harmless */

    seq_t k = 0;
    idx_t count = 0;
    while (true) {
        if (has_adjacent13(k) && !has_adjacent13(k + (seq_t)1)) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
}
