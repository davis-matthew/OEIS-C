/*
 * A122071  Sum over divisors d of 2n+1 of Kronecker(-18/d).
 * https://oeis.org/A122071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(2*n+1) where b(m) is multiplicative with b(3^e)=1,
 *            b(p^e)=e+1 if p≡1 or 3 (mod 8), b(p^e)=(1+(-1)^e)/2 if p≡5 or 7
 *            (mod 8); 2 is never present because m=2*n+1 is odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* m = 2*n + 1 (odd) */
    uint64_t m = (uint64_t)2 * (uint64_t)n + 1U;

    /* factor m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 3U)
            continue;              /* b(3^e) = 1 */

        switch (p % 8U)
        {
        case 1U:
        case 3U:
            result *= (seq_t)(e + 1U);
            break;

        case 5U:
        case 7U:
            if (e & 1U)           /* odd exponent → factor 0 */
                return (seq_t)0;
            /* even exponent → factor 1, no change */
            break;

        default:
            /* should never happen for odd primes */
            break;
        }
    }

    return result;
}
