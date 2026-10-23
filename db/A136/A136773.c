/*
 * A136773  n! never ends in this many 0's in base 13.
 * https://oeis.org/A136773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th natural number that never occurs as v13(n!) (the
 *            exponent of 13 in n!). v13(n!) increases by the number of times
 *            13 divides n at each step; all jumps > 1 skip one or more
 *            values, which form the sequence
 * Range:     u8 n <= 19, u16 n <= 5039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1, but harmless */
        return 0;

    /* current exponent of 13 in n! (v13(n!)) */
    unsigned long long v = 0;
    /* counter of missing values found so far */
    unsigned long long found = 0;
    /* iterate over n = 1, 2, 3, ... */
    for (unsigned long long x = 2; ; ++x)
    {
        /* exponent of 13 dividing current x */
        unsigned long long t   = x;
        unsigned int e         = 0;
        while ((t % 13ULL) == 0ULL)
        {
            t /= 13ULL;
            ++e;
        }

        unsigned long long prev_v = v;
        v += e;                     /* v now equals v13(x!) */

        if (e > 1)                  /* a jump of size e skips e-1 values */
        {
            for (unsigned int k = 1; k < e; ++k)
            {
                ++found;
                if (found == n)
                {
                    /* missing value is prev_v + k */
                    return (seq_t)(prev_v + (unsigned long long)k);
                }
            }
        }
        /* loop continues until we hit the requested n-th missing value */
    }
}
