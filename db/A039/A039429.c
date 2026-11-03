/*
 * A039429  Numbers whose base-10 representation has the same nonzero number
 *          of 5's and 7's.
 * https://oeis.org/A039429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for numbers whose base‑10 representation
   contains the same non‑zero number of 5's and 7's.  */
#include <stdint.h>

static inline int has_equal_nonzero_5_and_7(idx_t x)
{
    int c5 = 0, c7 = 0;
    while (x > 0) {
        int d = x % 10;
        if (d == 5) ++c5;
        else if (d == 7) ++c7;
        x /= 10;
    }
    return (c5 > 0 && c5 == c7);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cur = 1;

    while (1) {
        if (has_equal_nonzero_5_and_7(cur)) {
            ++count;
            if (count == n) return (seq_t)cur;
        }
        ++cur;
    }
}
