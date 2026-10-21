/*
 * A070920  a(n) = Card{ (x,y,z,u) | lcm(x,y,z,u)=n }.
 * https://oeis.org/A070920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = ∏_{p^e‖n} ((e+1)^4 – e^4) (multiplicative)
 * Range:     u8 n <= 11, u16 n <= 239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count the 4‑tuples (x,y,z,u) with lcm(x,y,z,u)=n */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1, but guard against zero */

    seq_t result = 1;
    idx_t m = n;

    /* Trial‑division factorisation – fast enough for n≤10000 */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0) { m /= p; ++e; }

            /* compute ((e+1)^4 – e^4) in 64‑bit unsigned */
            uint64_t val = ((uint64_t)(e + 1)) * ((uint64_t)(e + 1)) *
                           ((uint64_t)(e + 1)) * ((uint64_t)(e + 1))
                           -
                           ((uint64_t)e) * ((uint64_t)e) *
                           ((uint64_t)e) * ((uint64_t)e);
            result *= (seq_t)val;
        }
    }

    /* remaining prime factor, if any, with exponent 1 */
    if (m > 1)
    {
        uint64_t val = ((uint64_t)2) * ((uint64_t)2) *
                       ((uint64_t)2) * ((uint64_t)2)
                       -
                       ((uint64_t)1) * ((uint64_t)1) *
                       ((uint64_t)1) * ((uint64_t)1);
        result *= (seq_t)val;
    }

    return result;
}
