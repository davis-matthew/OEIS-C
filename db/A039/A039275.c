/*
 * A039275  Numbers whose base-3 representation has the same nonzero number of
 *          0's and 1's.
 * https://oeis.org/A039275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039275: numbers whose base‑3 representation has the same non‑zero number
   of 0's and 1's.  This implementation searches integers in increasing
   order and returns the nth such integer (1‑based index). */
#include <stdbool.h>

static inline bool balanced_base3(idx_t v)
{
    if (v == 0)          /* 0 has no non‑zero digits */
        return false;

    unsigned zeros = 0, ones = 0;
    while (v)
    {
        unsigned d = v % 3;
        if (d == 0)  ++zeros;
        else if (d == 1) ++ones;
        v /= 3;
    }
    return zeros > 0 && ones > 0 && zeros == ones;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n<1, return 0 */

    idx_t count = 0;
    for (idx_t v = 1; ; ++v)
    {
        if (balanced_base3(v))
        {
            ++count;
            if (count == n)
                return (seq_t)v;
        }
    }
}
