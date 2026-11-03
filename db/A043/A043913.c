/*
 * A043913  Numbers k such that 0 and 3 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true iff the base‑7 representation of x contains adjacent
   digits 0 and 3 (in either order). */
static bool has_adj_0_3(seq_t x)
{
    /* Store base‑7 digits in reverse order.  64 digits are more than
       enough for the values that appear in this sequence at modest
       indices, and the algorithm works for all sizes supported by the
       big‑integer type. */
    uint8_t digits[64];
    size_t len = 0;

    if (x == 0) return false;

    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 7);
        tmp /= 7;
    }

    if (len < 2) return false;

    for (size_t i = 1; i < len; ++i) {
        if ((digits[i-1] == 0 && digits[i] == 3) ||
            (digits[i-1] == 3 && digits[i] == 0))
            return true;
    }
    return false;
}

/* Sequence A043913: numbers k such that 0 and 3 occur juxtaposed in
   the base‑7 representation of k but not of k+1.
   Computed by a simple brute‑force search. */
seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t k = 1;

    while (true) {
        if (has_adj_0_3(k) && !has_adj_0_3(k + 1)) {
            ++seen;
            if (seen == n)
                return k;
        }
        ++k;
    }
}
