/*
 * A020652  Numerators in canonical bijection from positive integers to
 *          positive rationals.
 * https://oeis.org/A020652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, core, nice, tabf
 * Formula:   a(n) is the numerator of the reduced fraction at position n when
 *            all positive fractions p/q are ordered by increasing sum p+q,
 *            then increasing numerator p
 * Range:     u8 n <= 9283, u16 n <= 9283, u32 n <= 9283, u64 n <= 9283,
 *            u128 n <= 9283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t count = 0;

    for (idx_t s = 2; ; ++s)
    {
        for (idx_t p = 1; p < s; ++p)
        {
            if (gcd_u64(p, s) == 1)
            {
                ++count;
                if (count == n)
                    return (seq_t)p;
            }
        }
    }
}
