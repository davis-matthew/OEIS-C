/*
 * A024162  Number of incongruent integer-sided triangles with sides a,b,c,
 *          a<b<c, a+b+c=n such that gcd(a,b,c)=1.
 * https://oeis.org/A024162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 1776, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A024162 – number of incongruent primitive integer‑sided
 * triangles with perimeter n.
 * The function enumerates all (a,b,c) with 1 ≤ a<b<c, a+b+c=n,
 * checks the triangle inequality a+b>c, and counts those with
 * gcd(a,b,c)=1.  A fast Euclidean gcd on 64‑bit integers is used,
 * which suffices because all sides are < n ≤ 2³¹ in the tests.
 */
#include <stdbool.h>

static inline unsigned long long gcd_u64(unsigned long long x,
                                         unsigned long long y)
{
    while (y) {
        unsigned long long r = x % y;
        x = y;
        y = r;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Perimeter < 3 cannot form a triangle */
    if (n < 3)
        return 0;

    seq_t count = 0;

    /* a ranges from 1 up to floor(n/3) */
    for (idx_t a = 1; a <= n / 3; ++a) {
        /* b must satisfy a < b and b < c = n-a-b
         * Thus b ≤ (n - a - 1)/2.  We loop over this whole range
         * and filter by the triangle inequality later. */
        idx_t max_b = (n - a - 1) / 2;
        for (idx_t b = a + 1; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (c <= b)          /* enforce a < b < c */
                continue;

            /* triangle inequality a + b > c */
            if (a + b <= c)
                continue;

            /* GCD of a, b, c using u64 Euclidean algorithm */
            unsigned long long g = gcd_u64(a, b);
            g = gcd_u64(g, c);
            if (g == 1)
                ++count;
        }
    }

    return count;
}
