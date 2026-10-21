/*
 * A081874  Ordered odd short legs of primitive Pythagorean triangles.
 * https://oeis.org/A081874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth odd short leg of a primitive Pythagorean
 *            triangle (sorted ascending, duplicates allowed). The odd short
 *            leg is the smaller odd leg a in the Euclid parameterisation m>n,
 *            gcd(m,n)=1, (m−n) odd, a = m²−n² < 2mn. Each primitive triple
 *            contributes one occurrence of a; a may occur sever...
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Greatest‑common‑divisor for unsigned 64‑bit values. */
static unsigned long long
gcd64(unsigned long long a, unsigned long long b)
{
    while (b != 0) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the nth term of sequence A081874. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0; safety value */

    idx_t count = 0;                  /* how many terms have been seen */
    unsigned long long val = 3;       /* odd numbers only, start at 3 */

    while (1) {
        /* Count how many primitive triples have this odd number as
           the odd short leg.  Each representation a = u·v with
           u < v, gcd(u,v)=1, and v−u > √2·u corresponds to one such triple. */
        idx_t mult = 0;
        for (unsigned long long u = 1; u * u <= val; ++u) {
            if (val % u != 0)
                continue;                 /* u is not a divisor of val */

            unsigned long long v = val / u;
            if (u >= v)
                continue;                 /* we need u < v to avoid double counting */

            if (gcd64(u, v) != 1)
                continue;                 /* m,n must be coprime */

            unsigned long long d = v - u;        /* v−u */
            if (d * d > 2 * u * u)               /* v−u > √2·u  <=>  (v−u)² > 2u² */
                ++mult;
        }

        if (mult) {
            if (count + mult >= n)              /* the nth term lies in this group */
                return (seq_t)val;
            count += mult;
        }

        val += 2;                              /* next odd number */
    }
}
