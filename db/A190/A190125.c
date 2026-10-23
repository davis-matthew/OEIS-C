/*
 * A190125  If n = product(p_i^e_i), a(n) = product((p_i^e_i)^(p_i^e_i)).
 * https://oeis.org/A190125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} (p^e)^(p^e) (multiplicative over prime
 *            powers)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   _floor_isqrt_u64, big_pow, build_primes_upto, seq_mul, seq_one,
 *            seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_one.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return seq_one();

    /* Keep a copy for factorisation */
    uint64_t nn = (uint64_t)n;
    seq_t result = seq_one();

    /* Generate primes up to sqrt(n) */
    idx_t limit = _floor_isqrt_u64(nn);
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt && nn > 1; ++i) {
        uint64_t p = primes[i];
        if (nn % p == 0) {
            /* Count exponent e, factor out */
            unsigned e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            /* pp = p^e */
            seq_t pp = seq_pow((seq_t)p, e);
            /* part = pp^pp */
            seq_t part = big_pow(pp, pp);
            /* Multiply into result */
            result = seq_mul(result, part);
        }
    }

    /* Handle remaining prime factor > sqrt(original n) */
    if (nn > 1) {
        seq_t pp = seq_pow((seq_t)nn, 1);        /* pp = nn (prime power 1) */
        seq_t part = big_pow(pp, pp);            /* part = pp^pp */
        result = seq_mul(result, part);
    }

    free(primes);
    return result;
}
