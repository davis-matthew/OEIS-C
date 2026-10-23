/*
 * A120098  Short leg of primitive Pythagorean triangle corresponding to
 *          hypotenuse A094194.
 * https://oeis.org/A120098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(m^2 - n^2, 2*m*n) for the nth pair (m,n) of coprime
 *            integers with opposite parity, enumerated by increasing m and
 *            increasing n
 * Range:     u8 n <= 30, u16 n <= 7332, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: short leg of primitive Pythagorean triangle corresponding to
   hypotenuse A094194. */

#include <stdint.h>

/* Euclidean gcd for integers up to a few thousand; used for the small m,n. */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for offset 1 but safe */

    idx_t count = 0;

    for (idx_t m = 2; ; ++m) {
        for (idx_t nn = 1; nn < m; ++nn) {
            /* require m and n to have opposite parity */
            if (((m ^ nn) & 1ULL) == 0) continue;

            /* require m and n to be coprime */
            if (gcd64(m, nn) != 1) continue;

            /* compute the two legs */
            seq_t leg1 = (seq_t)m * m - (seq_t)nn * nn;   /* m^2 - n^2 */
            seq_t leg2 = (seq_t)2 * m * nn;               /* 2mn */

            seq_t short_leg = (leg1 < leg2) ? leg1 : leg2;

            ++count;
            if (count == n) return short_leg;
        }
    }
}
