/*
 * A045789  Squares with initial digit '6'.
 * https://oeis.org/A045789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   leading_digit_is6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A045789: squares whose first decimal digit is 6.
   Equivalent to a(n) = A045860(n)^2, where A045860(n) is the n-th integer
   whose square starts with 6.  The function generates successive integers,
   squares them, and checks the leading digit until the n-th such square is
   found.  No caching is used; each call is independent. */

#include <stdbool.h>
#include "../utils/leading_digit_is6.h"

seq_t a(idx_t n)
{
    if (n == 0) {                 /* behaviour below the offset is unspecified */
        return 0;
    }

    idx_t count = 0;
    idx_t m = 1;                  /* candidate integer */

    while (count < n) {
        seq_t sq = (seq_t)m * (seq_t)m;       /* square in seq_t */
        if (leading_digit_is6(sq)) {
            ++count;
            if (count == n) {
                return sq;
            }
        }
        ++m;
    }

    /* Should never reach here */
    return 0;
}
