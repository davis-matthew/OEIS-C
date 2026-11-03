/*
 * A039421  Numbers whose base-10 representation has the same nonzero number
 *          of 3's and 8's.
 * https://oeis.org/A039421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 8102 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑10 representation has the same non‑zero
   number of 3's and 8's.  */
#include <stdint.h>
#include <stdbool.h>

/* Return true if x has equal non‑zero counts of digits 3 and 8.  */
static inline bool equal_nonzero_38(uint64_t x)
{
    int c3 = 0, c8 = 0;
    while (x != 0U) {
        uint8_t d = (uint8_t)(x % 10U);
        if (d == 3U) ++c3;
        else if (d == 8U) ++c8;
        x /= 10U;
    }
    return (c3 == c8) && (c3 > 0);
}

/*  OEIS A039421: Numbers whose base‑10 representation has the
    same nonzero number of 3's and 8's.  */
seq_t a(idx_t n)
{
    if (n == 0U) {
        /* Offset is 1; undefined behavior for n = 0, return 0. */
        return (seq_t)0;
    }

    idx_t found = 0U;
    uint64_t cur   = 1U;            /* start from the first positive integer */

    while (true) {
        if (equal_nonzero_38(cur)) {
            ++found;
            if (found == n) {
                return (seq_t)cur;
            }
        }
        ++cur;
    }
}
