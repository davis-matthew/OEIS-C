/*
 * A039235  Numbers whose base-12 representation has the same number of 2's
 *          and 8's.
 * https://oeis.org/A039235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose base‑12 representation
 *            has an equal number of 2’s and 8’s. Brute‑force search
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_equal_2_8(seq_t x)
{
    unsigned c2 = 0;
    unsigned c8 = 0;
    while (x)
    {
        unsigned d = (unsigned)(x % 12);
        if (d == 2)
            ++c2;
        else if (d == 8)
            ++c8;
        x /= 12;
    }
    return c2 == c8;
}

seq_t a(idx_t n)
{
    seq_t cand = 0;
    idx_t found = 0;
    while (1)
    {
        if (has_equal_2_8(cand))
        {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
}
