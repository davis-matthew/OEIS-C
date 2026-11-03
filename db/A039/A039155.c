/*
 * A039155  Numbers whose base-10 representation has the same number of 8's
 *          and 9's.
 * https://oeis.org/A039155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non‑negative integer whose decimal
 *            representation contains the same number of digits 8 and 9. a(1)
 *            = 0, a(2) = 1, a(3) = 2, …
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x has equal numbers of 8′s and 9′s in its base‑10
   representation. */
static inline bool equal_8_9(seq_t x)
{
    int c8 = 0;
    int c9 = 0;

    /* 0 contains no 8s and no 9s – it is a valid member. */
    if (x == 0)
        return true;

    seq_t y = x;
    while (y > 0) {
        int d = (int)(y % 10);
        if (d == 8)
            c8++;
        else if (d == 9)
            c9++;
        y = y / 10;
    }
    return c8 == c9;
}

/* External‑linkage function requested by the build. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand  = 0;

    while (count < n) {
        if (equal_8_9(cand))
            count++;
        if (count == n)
            return cand;
        cand = cand + 1;
    }
    /* unreachable, but keep compiler happy */
    return cand;
}
