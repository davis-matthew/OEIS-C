/*
 * A043415  Numbers whose base-7 representation contains exactly three 5's.
 * https://oeis.org/A043415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑7 representation
 *            contains exactly three 5's. This implementation performs a
 *            simple search, counting digits as it scans the integers
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline int count_fives_in_base7(seq_t x)
{
    int cnt = 0;
    while (x > 0) {
        if ((x % 7) == 5)
            ++cnt;
        x /= 7;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for the OEIS offset, defensive return */
        return 0;

    idx_t found = 0;
    seq_t val   = 0;

    while (found < n) {
        ++val;
        if (count_fives_in_base7(val) == 3)
            ++found;
    }
    return val;
}
