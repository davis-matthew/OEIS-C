/*
 * A044560  Numbers k such that string 5,6 occurs in the base 7 representation
 *          of k but not of k+1.
 * https://oeis.org/A044560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k (1‑based) whose base‑7 representation
   contains the adjacent digits 5,6, whereas the base‑7 representation
   of k+1 does not.  The function searches sequentially and
   returns the value for any requested n. */

#include <stdbool.h>
#include <stddef.h>

/* Test whether a base‑7 representation of v contains the
   adjacent digits 5 followed by 6. */
static bool has56_in_base7(seq_t v)
{
    bool prev_is_6 = false;
    while (v != 0) {
        unsigned d = (unsigned)(v % 7);
        if (prev_is_6 && d == 5) return true;
        prev_is_6 = (d == 6);
        v = v / 7;
    }
    return false;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t k = 0;; ++k) {
        if (has56_in_base7(k) && !has56_in_base7(k + (seq_t)1)) {
            ++found;
            if (found == n) return k;
        }
    }
}
