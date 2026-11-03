/*
 * A039389  Numbers whose base-9 representation has the same nonzero number of
 *          5's and 8's.
 * https://oeis.org/A039389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers with equal non‑zero numbers of 5’s and 8’s in base 9
 * Range:     u8 n <= 6, u16 n <= 7663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9907 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* helper: counts 5's and 8's in the base‑9 expansion of x */
static inline bool has_equal_58(seq_t x)
{
    int cnt5 = 0, cnt8 = 0;
    while (x > 0) {
        int d = (int)(x % 9);
        if (d == 5) ++cnt5;
        else if (d == 8) ++cnt8;
        x /= 9;
    }
    return cnt5 > 0 && cnt5 == cnt8;
}

seq_t a(idx_t n)
{
    seq_t x = 1;          /* start checking from 1 */
    idx_t found = 0;      /* how many valid numbers seen */

    for (;; ++x) {
        if (has_equal_58(x)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
