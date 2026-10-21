/*
 * A070208  Number of integer triangles with perimeter n having integral area
 *          but not integral inradius.
 * https://oeis.org/A070208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051516(n) – A070201(n) Number of integer triangles with
 *            perimeter n having integral area but not integral inradius
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Count triangles with perimeter n having integer area and non‐integer inradius */
seq_t a(idx_t n)
{
    /* Fast return for impossible perimeters */
    if (n < 3) return 0;

    uint64_t N = (uint64_t)n;       /* unsigned 64‑bit copy of n */
    seq_t have_area = 0;            /* triangles with integral area */
    seq_t have_inc  = 0;            /* triangles with integral area and integral inradius */

    /* Loop over the smallest side a (1 <= a <= n/3) */
    for (uint64_t a = 1; a <= N / 3; ++a) {
        /* b must satisfy a <= b <= (n-a)/2  and   a+b > n/2 */
        uint64_t halfN = N / 2;
        uint64_t b_min = a;
        uint64_t tmp   = halfN - a + 1;   /* from a+b > n/2  ->  b > n/2 - a */
        if (tmp > b_min) b_min = tmp;

        uint64_t b_max = (N - a) / 2;
        if (b_min > b_max) continue;

        for (uint64_t b = b_min; b <= b_max; ++b) {
            uint64_t c = N - a - b;   /* third side, automatically c >= b */

            /* Compute the product n*(n-2a)*(n-2b)*(n-2c) */
            seq_t u = (seq_t)(N - 2 * a);
            seq_t v = (seq_t)(N - 2 * b);
            seq_t w = (seq_t)(N - 2 * c);

            seq_t z = (seq_t)N;
            z *= u;
            z *= v;
            z *= w;   /* z = n*(n-2a)*(n-2b)*(n-2c) */

            /* sqrt(z) must be integer and divisible by 4 for integer area */
            seq_t t = floor_sqrt(z);
            if (t * t != z) continue;          /* not a perfect square */
            if ((t & 3) != 0) continue;        /* not divisible by 4 */

            ++have_area;                       /* integral area */

            /* Check if inradius is integral: t % (2*n) == 0  (t = 4*area) */
            seq_t twoN = (seq_t)2 * (seq_t)N;
            if (t % twoN == 0) ++have_inc;     /* integral inradius */
        }
    }

    return have_area - have_inc;
}
