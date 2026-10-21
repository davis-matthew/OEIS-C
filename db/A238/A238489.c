/*
 * A238489  Numbers k such that k+x+y is a square, where x and y are the two
 *          squares nearest to k.
 * https://oeis.org/A238489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that k + x + y is a square,
 *            where x and y are the nearest squares to k (for k a square x is
 *            the previous square, y = k). The function enumerates intervals
 *            [s²,(s+1)²) and tests k = s² (square case) and k = s² + d , 1 ≤
 *            d ≤ 2s (non‑square case), with the condition t...
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* s is the floor of √k.  k is always in [s²,(s+1)²). */
    idx_t s = 0;               /* current s (≥0) */
    idx_t count = 0;           /* how many terms have been found so far */

    while (1)
    {
        /* s == 0 gives the unique term 0. */
        if (s == 0)
        {
            ++count;
            if (count == n)
                return (seq_t)0;
            ++s;
            continue;
        }

        /* Common values for this s. */
        seq_t ss = (seq_t)s * (seq_t)s;          /* s² */

        /* -------- 1. Square case : k = s² -------- */
        seq_t expr = (seq_t)3 * ss - (seq_t)2 * (seq_t)s + (seq_t)1;
        seq_t r = floor_sqrt(expr);
        if (r * r == expr)
        {
            ++count;
            if (count == n)
                return ss;
        }

        /* -------- 2. Non‑square case : k = s² + d, 1 ≤ d ≤ 2s -------- */
        seq_t m = (seq_t)3 * ss + (seq_t)2 * (seq_t)s + (seq_t)1; /* base for t² */

        seq_t low  = m + (seq_t)1;                 /* smallest t² to check */
        seq_t high = m + (seq_t)2 * (seq_t)s;      /* largest t² to check */

        seq_t t_min = floor_sqrt(low);
        if (t_min * t_min < low)
            ++t_min;                               /* ceil(sqrt(low)) */

        seq_t t_max = floor_sqrt(high);

        for (seq_t t = t_min; t <= t_max; ++t)
        {
            seq_t d = t * t - m;                   /* 1 ≤ d ≤ 2s automatically */
            seq_t k = ss + d;
            ++count;
            if (count == n)
                return k;
        }

        ++s;   /* move to the next interval */
    }
}
