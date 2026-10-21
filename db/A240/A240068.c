/*
 * A240068  Number of prime Lipschitz quaternions having norm prime(n).
 * https://oeis.org/A240068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * (prime(n) + 1)
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_prime_limit_for_idx.h"

seq_t a(idx_t n)
{
    /* static cache of primes up to the largest n requested so far */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (n == 0)
        return (seq_t)0;                /* undefined but harmless */

    if (primes_cnt < (size_t)n)
    {
        size_t limit = _prime_limit_for_idx(n);
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0u;

        build_primes_upto(limit, &new_primes, &new_cnt);

        /* replace old cache if it exists */
        if (primes)
            free(primes);
        primes = new_primes;
        primes_cnt = new_cnt;
    }

    seq_t prime_value = (seq_t)primes[n - 1];
    seq_t result = prime_value + 1;
    result *= (seq_t)8;
    return result;
}
