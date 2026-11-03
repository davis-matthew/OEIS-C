/*
 * A043148  Numbers k such that 3 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043148
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

/* Returns true if the base‑7 representation of x has the substring
 * "36" or "63". */
static bool has_adjacent_36_or_63(seq_t x)
{
    if (x == 0)
        return false;            /* "0" contains neither 3 nor 6 */

    uint8_t digits[64];
    int len = 0;
    seq_t tmp = x;

    while (tmp != 0) {
        digits[len++] = (uint8_t)(tmp % 7);
        tmp /= 7;
    }

    /* digits[0] is least significant, digits[len-1] most significant. */
    for (int i = 0; i < len - 1; ++i) {
        uint8_t d1 = digits[i];
        uint8_t d2 = digits[i + 1];
        if ((d1 == 3 && d2 == 6) || (d1 == 6 && d2 == 3))
            return true;
    }
    return false;
}

/* a(n): the n‑th integer k whose base‑7 representation contains the
 * adjacent digits 3 and 6 in either order, while k-1 does not. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* out of range, shouldn't happen */

    seq_t k = 1;
    idx_t count = 0;

    while (true) {
        if (has_adjacent_36_or_63(k) && !has_adjacent_36_or_63(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
