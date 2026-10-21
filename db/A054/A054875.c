/*
 * A054875  Number of pairwise incongruent triangles with integer sides and
 *          positive integer area and longest side of length n.
 * https://oeis.org/A054875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3314, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A054875: number of pairwise incongruent triangles with
 * integer sides, positive integer area and longest side of length n.
 * For each n the algorithm iterates over all admissible side pairs
 * (a,b) with a ≤ b ≤ n and a + b > n, checks whether the Heron
 * product 16·area² = (a+b+n)(b+n-a)(a+n-b)(a+b‑n) is a perfect
 * square, and counts such triangles.  The routine runs in O(n²)
 * time and uses only 64‑bit arithmetic (see below for overflow safety). */

#include <stdint.h>
#include <math.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 1)                /* offset starts at 1 */
        return 0;

    seq_t cnt = 0;            /* result type */

    for (idx_t a = 1; a <= n; ++a)
    {
        /* b must satisfy a ≤ b ≤ n and a + b > n;
           the minimal b is max(a, n - a + 1) */
        idx_t b_start;
        idx_t tmp = n - a + 1;
        b_start = (tmp > a) ? tmp : a;

        for (idx_t b = b_start; b <= n; ++b)
        {
            /* Compute the four terms of 16·area² */
            uint64_t P1 = (uint64_t)a + (uint64_t)b + (uint64_t)n;
            uint64_t P2 = (uint64_t)b + (uint64_t)n - (uint64_t)a;
            uint64_t P3 = (uint64_t)a + (uint64_t)n - (uint64_t)b;
            uint64_t P4 = (uint64_t)a + (uint64_t)b - (uint64_t)n;

            /* Product fits in 64‑bit for n ≤ 10000
               (upper bound: (3n)⁴ ≤ 8.1 × 10¹⁷ < 2⁶⁴) */
            uint64_t prod = P1;
            prod *= P2;
            prod *= P3;
            prod *= P4;

            /* Perfect‑square test using long double sqrt for
               sufficient precision up to 10¹⁸ */
            uint64_t r = (uint64_t)(sqrtl((long double)prod) + 0.5L);
            if (r * r == prod)
                ++cnt;
        }
    }

    return cnt;
}
