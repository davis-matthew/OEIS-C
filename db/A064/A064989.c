/*
 * A064989  Multiplicative with a(2^e) = 1 and a(p^e) = prevprime(p)^e for odd
 *          primes p.
 * https://oeis.org/A064989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, mult
 * Formula:   a(n) = product over all odd prime powers p^e || n of
 *            prevprime(p)^e (with a(2^e) = 1)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"   /* static void factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/prev_prime.h"   /* idx_t prev_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined, safety guard */

    /* local storage for prime factors */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;

    /* factorize n */
    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t prime  = pr[i];
        uint64_t e      = exp[i];

        if (prime == 2)                 /* skip powers of 2 */
            continue;

        seq_t base  = (seq_t)prev_prime((idx_t)prime);
        seq_t powv  = 1;

        /* binary exponentiation (seq_t arithmetic) */
        while (e)
        {
            if (e & 1ULL)
                powv *= base;
            if (e > 1ULL)
                base *= base;
            e >>= 1;
        }

        result *= powv;
    }
    return result;
}
