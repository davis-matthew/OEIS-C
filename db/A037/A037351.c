/*
 * A037351  Numbers whose base-3 and base-10 expansions have no digits in
 *          common.
 * https://oeis.org/A037351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose decimal and base‑3 expansions
 *            have no digit in common (the only possible common digits are 0,
 *            1, 2)
 * Range:     u8 n <= 75, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_good(seq_t x)
{
    unsigned dec_mask = 0;        /* bits 0‑2 indicate which of {0,1,2} appear in decimal */
    seq_t y = x;

    if (y == 0) {
        dec_mask |= 1;           /* decimal digit 0 present */
    } else {
        while (y > 0) {
            unsigned d = (unsigned)(y % (seq_t)10);
            if (d < 3) dec_mask |= 1 << d;
            y /= (seq_t)10;
        }
    }

    y = x;
    if (y == 0) {
        if (dec_mask & 1) return false;        /* base‑3 digit 0 present and also in decimal */
    } else {
        while (y > 0) {
            unsigned t = (unsigned)(y % (seq_t)3);   /* base‑3 digit */
            if (dec_mask & (1 << t)) return false;   /* intersection found */
            y /= (seq_t)3;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* the sequence is defined for n ≥ 1 */

    idx_t found = 0;
    seq_t candidate = 3;            /* the sequence starts with 3 */

    while (found < n) {
        if (is_good(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
    return candidate; /* unreachable, but satisfies compiler */
}
