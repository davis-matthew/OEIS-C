/*
 * A097266  Number of primitive Pythagorean quadruples with diagonal 2n+1.
 * https://oeis.org/A097266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 702, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A097266:  Number of primitive Pythagorean quadruples with diagonal 2n+1
   The function counts unordered positive solutions (a,b,c,d) with  a^2+b^2+c^2 = d^2,
   d = 2n+1 odd, gcd(a,b,c,d) = 1, and a ≤ b ≤ c.  */
#include <stdint.h>
#include <math.h>
#include "../utils/gcd_uint64.h"

seq_t a(idx_t n)
{
    /*  Convert n to a 64‑bit unsigned integer for intermediate arithmetic
        (n itself is small enough for that – n ≤ 10000 in our tests).            */
    uint64_t dn = (uint64_t)n;
    uint64_t d  = 2u * dn + 1u;        /* diagonal, always odd                     */
    uint64_t d2 = d * d;              /* d squared, fits in 64 bits (max ≈4·10⁸)    */

    uint64_t count = 0;                /* accumulate the answer in 64‑bit        */

    for (uint64_t a = 1; a < d; ++a)
    {
        uint64_t a2 = a * a;
        if (a2 >= d2) break;           /* no room for positive b and c           */

        uint64_t remain = d2 - a2;     /* b² + c²                                 */

        /*  From a² + b² + c² = d² and a ≤ b ≤ c, we have
            b² ≤ (d² – a²)/2.  This bound cuts the inner loop about √2 .     */
        uint64_t bmax = (uint64_t)sqrt((double)remain / 2.0L);
        if (bmax < a) continue;        /* b must be at least a                   */

        for (uint64_t b = a; b <= bmax; ++b)
        {
            uint64_t b2 = b * b;
            uint64_t s  = d2 - a2 - b2;     /* c²                            */
            if (s < b2) continue;          /* ensures c ≥ b                   */

            uint64_t c = (uint64_t)sqrt((double)s);  /* nearest lower integer  */
            if (c * c != s) continue;        /* not a perfect square            */
            if (c < b) continue;             /* keep ordering a ≤ b ≤ c         */

            /*  Primitive check: gcd(a,b,c) must be 1                       */
            uint64_t g = gcd_uint64(a, gcd_uint64(b, c));
            if (g == 1u) ++count;
        }
    }
    return (seq_t)count;
}
