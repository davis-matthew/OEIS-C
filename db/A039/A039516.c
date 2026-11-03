/*
 * A039516  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 5's.
 * https://oeis.org/A039516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑12 representation has the same
 *            nonzero count of 2's and 5's
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 987 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool same_25(seq_t x)
{
    idx_t c2 = 0, c5 = 0;
    while (x > (seq_t)0)
    {
        uint64_t d = (uint64_t)(x % (seq_t)12);
        if (d == 2)
            ++c2;
        else if (d == 5)
            ++c5;
        x /= (seq_t)12;
    }
    return (c2 > 0) && (c2 == c5);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t found = 0;
    seq_t num = (seq_t)1;
    for (;; ++num)
    {
        if (same_25(num))
        {
            ++found;
            if (found == n)
                return num;
        }
    }
}
