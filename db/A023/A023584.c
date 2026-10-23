/*
 * A023584  Greatest exponent in prime-power factorization of 2*prime(n)-1.
 * https://oeis.org/A023584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest exponent in prime-power factorization of
 *            2*prime(n)-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

static bool _primes_initialized = false;
static uint64_t *_primes = NULL;
static size_t _prime_cnt = 0;

/* initialise a list of primes sufficient for all foreseeable n */
static void _init_primes(void)
{
    if (_primes_initialized)
        return;
    /* upper bound: for n=10000, prime(n)≈9.2e4, sqrt(2*prime-1)≈430,
       so 500000 is far more than enough. */
    const idx_t LIMIT = 500000;
    build_primes_upto(LIMIT, &_primes, &_prime_cnt);
    _primes_initialized = true;
}

seq_t a(idx_t n)
{
    _init_primes();

    if (n < 1)
        return (seq_t)0;  /* undefined, but harmless */

    /* compute m = 2 * prime(n) - 1 */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t m = 2 * p - 1;

    uint64_t temp = m;
    uint32_t max_exp = 0;

    for (size_t i = 0; i < _prime_cnt &&
         (uint64_t)_primes[i] * (uint64_t)_primes[i] <= temp; ++i)
    {
        uint64_t q = _primes[i];
        if (temp % q == 0)
        {
            uint32_t e = 0;
            do
            {
                temp /= q;
                ++e;
            } while (temp % q == 0);
            if (e > max_exp)
                max_exp = e;
        }
    }

    /* if a prime factor > sqrt(m) remains, its exponent is 1 */
    if (temp > 1ULL && max_exp < 1)
        max_exp = 1;

    return (seq_t)max_exp;
}
