/*
 * A102095  Greatest edge length of a cuboid having integer edge lengths,
 *          volume n and minimal surface area under those restrictions.
 * https://oeis.org/A102095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   int_sqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) for OEIS sequence A102095:
 * Find all integer triples (a,b,c) with a*b*c=n that minimize a+b+c.
 * Return the largest edge length among those triples (i.e., the maximum of a,b,c).
 * This is equivalent to minimizing the surface area of the cuboid for a fixed volume.
 */

#include <stdint.h>
#include "../utils/int_sqrt_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but keep safe */

    idx_t bestSum = (idx_t)-1;            /* large */
    idx_t bestC   = 0;

    idx_t sqrt_n  = int_sqrt_idx(n);      /* floor(sqrt(n)) */

    for (idx_t a = 1; a <= sqrt_n; ++a)
    {
        if (n % a != 0) continue;
        idx_t n1 = n / a;
        idx_t sqrt_n1 = int_sqrt_idx(n1);

        for (idx_t b = a; b <= sqrt_n1; ++b)
        {
            if (n1 % b != 0) continue;
            idx_t c = n1 / b;          /* c >= b >= a */

            idx_t sum = a + b + c;
            if (sum < bestSum || (sum == bestSum && c > bestC))
            {
                bestSum = sum;
                bestC   = c;
            }
        }
    }

    return (seq_t)bestC;
}
