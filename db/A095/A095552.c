/*
 * A095552  a(n) = 80 written in base n.
 * https://oeis.org/A095552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 80 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case: base‑1 (unary) uses 80 ones. */
    if (n == 1) {
        seq_t r = 0;
        for (idx_t i = 0; i < 80; ++i) {
            r = r * (seq_t)10 + (seq_t)1;
        }
        return r;
    }

    /* Compute the base‑n digits of 80 (reminder, 80 < 10^3). */
    idx_t val = (idx_t)80;
    /* Maximum length for base 2 representation of 80 is 7, 8 slots are safe. */
    idx_t rev[8];
    size_t len = 0;
    while (val > 0) {
        rev[len++] = val % n;
        val /= n;
    }

    /* Build the decimal number by concatenating the digits. */
    seq_t res = 0;
    for (size_t i = len; i-- > 0; ) {
        idx_t d = rev[i];
        if (d < 10) {
            res = res * (seq_t)10 + (seq_t)d;
        } else {
            /* d ≤ 80, thus fits into two decimal digits. */
            res = res * (seq_t)100 + (seq_t)d;
        }
    }
    return res;
}
