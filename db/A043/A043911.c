/*
 * A043911  Numbers k such that 0 and 1 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: enumerate integers k and collect those whose base-7 representation
   contains the digits 0 and 1 adjacent (in either order) but whose base-7
   representation of k+1 does not.  The n-th such k is returned. */
#include <stdbool.h>

static bool has_adjacent_zero_one(seq_t x)
{
    seq_t temp = x;
    if (temp == 0)
        return false;                     /* 0 has no digits 0 and 1 side by side */

    seq_t prev = 7;                       /* sentinel digit outside 0..6 */

    while (temp > 0) {
        seq_t d = temp % 7;               /* least significant digit */
        if ((prev == 0 && d == 1) ||
            (prev == 1 && d == 0))
            return true;                  /* adjacency found */

        prev = d;
        temp /= 7;                        /* shift right by one digit */
    }
    return false;                         /* no adjacency found */
}

seq_t a(idx_t n)
{
    idx_t count = 0;                      /* how many qualifying numbers found */
    seq_t k = 0;                          /* current candidate */

    while (count < n) {
        if (has_adjacent_zero_one(k) &&
            !has_adjacent_zero_one(k + (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
    /* The loop above always returns before reaching this point */
    return (seq_t)0;
}
