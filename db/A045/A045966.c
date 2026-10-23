/*
 * A045966  a(1)=3; if n = Product p_i^e_i, n > 1, then a(n) = Product
 *          p_{i+2}^e_i.
 * https://oeis.org/A045966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(1) = 3; for n>1, a(n) = Π (prime_i+2)^e_i, where n = Π
 *            prime_i^e_i
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, pow_uint, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/prime_pi.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1U)
        return 3;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p      = primes[i];
        uint64_t exp    = exps[i];

        /* position of p in the prime list – p is prime */
        seq_t  pi_seq  = prime_pi(p);          /* returns seq_t */
        idx_t  idx     = (idx_t)pi_seq;        /* safe: idx fits in 64 bits */

        /* the prime two positions ahead */
        uint64_t q = get_prime(idx + 2);

        /* multiply the corresponding power */
        seq_t pow = pow_uint(q, exp);
        res *= pow;
    }
    return res;
}
