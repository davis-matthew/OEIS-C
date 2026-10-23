/*
 * A027761  Numerator of sum_{p prime, p-1 divides 2*n} 1/p.
 * https://oeis.org/A027761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{p prime, p-1 | 2*n} 1/p
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 89, u64 n <= 269,
 *            u128 n <= 1889, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

static idx_t _prime_cache_limit = 0;
static uint64_t *_prime_cache = NULL;
static size_t _prime_cache_count = 0;

/* Ensure prime list up to the requested limit. */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= _prime_cache_limit)
        return;
    free(_prime_cache);
    build_primes_upto(limit, &_prime_cache, &_prime_cache_count);
    _prime_cache_limit = limit;
}

/* Simple primality test using the cached prime list. */
static bool _is_prime(const seq_t val)
{
    if (val < 2)
        return false;
    uint64_t v = (uint64_t)val;
    for (size_t i = 0; i < _prime_cache_count; ++i) {
        uint64_t p = _prime_cache[i];
        if (p * p > v)
            break;
        if (v % p == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but harmless */

    idx_t twoN = n * (idx_t)2;
    idx_t limit = twoN + (idx_t)1;          /* know p = d+1 <= twoN+1 */
    _ensure_primes_upto(limit);

    seq_t num = 0;  /* numerator of the sum */
    seq_t den = 1;  /* denominator of the sum */

    for (idx_t i = 1; i * i <= twoN; ++i) {
        if (twoN % i == 0) {
            idx_t d1 = i;
            idx_t d2 = twoN / i;

            seq_t p1 = (seq_t)(d1 + (idx_t)1);
            if (_is_prime(p1)) {
                seq_t new_num = num * p1 + den;
                seq_t new_den = den * p1;
                seq_t g = gcd(new_num, new_den);
                num = new_num / g;
                den = new_den / g;
            }

            if (d1 != d2) {
                seq_t p2 = (seq_t)(d2 + (idx_t)1);
                if (_is_prime(p2)) {
                    seq_t new_num = num * p2 + den;
                    seq_t new_den = den * p2;
                    seq_t g = gcd(new_num, new_den);
                    num = new_num / g;
                    den = new_den / g;
                }
            }
        }
    }
    return num;  /* numerator of the reduced fraction */
}
