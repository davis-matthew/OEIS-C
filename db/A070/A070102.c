/*
 * A070102  Number of obtuse integer triangles with perimeter n and relatively
 *          prime side lengths.
 * https://oeis.org/A070102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of obtuse integer triangles with perimeter n * and
 *            relatively prime side lengths (primitive obtuse triangles). For
 *            a * given n the function enumerates all sorted triples (a,b,c)
 *            with * a ≤ b ≤ c, a+b+c = n, a+b > c (triangle inequality) and
 *            gcd(a,b,c)=1. * For each triple it checks...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 3)        /* too small to form a triangle */
        return 0;

    uint64_t count = 0;
    /* c is the largest side.  The bounds are derived from
       a ≤ b ≤ c, a+b+c = n and a+b > c.                                            */
    for (idx_t c = (n + 2) / 3; c <= (n - 1) / 2; ++c)
    {
        /* For a given c, a must satisfy: */
        /*   a ≥ max(1, n‑2c)   to keep b = n‑c‑a ≤ c   */
        /*   a ≤ (n‑c)/2         to keep a ≤ b            */
        idx_t min_a = 1;
        if (n > 2 * c)
            min_a = n - 2 * c;
        idx_t max_a = (n - c) / 2;
        if (min_a > max_a)
            continue;

        uint64_t c2 = (uint64_t)c * c;
        for (idx_t a = min_a; a <= max_a; ++a)
        {
            idx_t b = n - c - a;   /* by construction a ≤ b ≤ c */

            /* Primitive triangle test: gcd(a,b,c) == 1 */
            uint64_t g = gcd_u64(a, b);
            g = gcd_u64(g, c);
            if (g != 1)
                continue;

            uint64_t a2 = (uint64_t)a * a;
            uint64_t b2 = (uint64_t)b * b;
            if (c2 > a2 + b2)          /* obtuse inequality */
                ++count;
        }
    }
    return (seq_t)count;
}
