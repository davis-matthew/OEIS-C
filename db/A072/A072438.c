/*
 * A072438  Remove prime factors of form 4*k+1.
 * https://oeis.org/A072438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of all prime powers of n whose primes are 2 or ≡3
 *            (mod 4). Equivalently, remove every factor p^k with p≡1 (mod 4)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    /* Ensure we have a prime table up to √n */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t  prime_limit = 0;

    idx_t limit = _floor_isqrt_u64(n);
    if (limit > prime_limit) {
        if (primes)
            free(primes);
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_limit = limit;
    }

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* Factor using primes up to √n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > temp / p)
            break;               /* p*p > temp */
        if (temp % p == 0) {
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (p % 4 != 1) {    /* keep this prime power */
                for (size_t k = 0; k < exp; ++k)
                    result *= (seq_t)p;
            }
        }
    }

    /* Remaining factor (if any) is prime and > √original n */
    if (temp > 1) {
        if ((temp % 4) != 1)
            result *= (seq_t)temp;
    }

    return result;
}
