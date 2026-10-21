/*
 * A087782  a(n) = number of solutions to x^3 + x == 0 (mod n).
 * https://oeis.org/A087782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = number of solutions to x^3 + x ≡ 0 (mod n) Computed
 *            multiplicatively: a(2 ) = 2, a(2^e) = 1 (e > 1) a(p^e) = 3 if p
 *            ≡ 1 (mod 4) (p odd prime) a(p^e) = 1 if p ≡ 3 (mod 4) (p odd
 *            prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"   /* provides static void factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1) return (seq_t)1;

    /* Factorise n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   pcnt = 0;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = 1;

    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t factor;

        if (p == 2ULL)
        {
            /* a(2^1) = 2, a(2^e) = 1 for e > 1 */
            factor = (e == 1) ? 2 : 1;
        }
        else
        {
            /* For odd primes */
            factor = (p % 4ULL == 1ULL) ? 3 : 1;
        }

        result *= factor;
    }

    return result;
}
