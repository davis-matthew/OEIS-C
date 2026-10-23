/*
 * A024157  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          such that n divides abc.
 * https://oeis.org/A024157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of integer-sided triangles with sides a<b<c, a+b+c=n,
   such that n divides a*b*c (OEIS A024157). */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* iterate over the smallest side a (1 <= a <= n/3) */
    for (idx_t a = 1; a <= n / 3; ++a) {
        /* b must satisfy a < b < c and a+b > c, where c = n-a-b.
           The upper bound ensures b < c, the triangle test ensures a+b>c. */
        idx_t b_max = (n - a - 1) / 2;          /* floor((n-a-1)/2) */
        for (idx_t b = a + 1; b <= b_max; ++b) {
            idx_t c = n - a - b;
            if (a + b <= c)          /* not a triangle */
                continue;

            /* Check divisibility n | a*b*c using 64‑bit mod to avoid overflow. */
            unsigned long long prod = ((unsigned long long)a * b) % (unsigned long long)n;
            prod = (prod * c) % (unsigned long long)n;
            if (prod == 0)
                ++count;
        }
    }

    return count;
}
