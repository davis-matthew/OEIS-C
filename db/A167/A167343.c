/*
 * A167343  Totally multiplicative sequence with a(p) = (p-1)^2 = p^2-2p+1 for
 *          prime p.
 * https://oeis.org/A167343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} ((p-1)^2)^e (totally multiplicative, a(p) =
 *            (p-1)^2)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1; undefined for n==0 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;           /* work in unsigned 64‑bit for factors */

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            unsigned e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            seq_t base = (seq_t)(p - 1);
            base = base * base;          /* (p-1)^2 */

            /* multiply base^e into result */
            for (unsigned i = 0; i < e; ++i)
                result *= base;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1)
    {
        seq_t base = (seq_t)(m - 1);
        base = base * base;              /* exponent 1 */
        result *= base;
    }

    return result;
}
