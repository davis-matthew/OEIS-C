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
 * Validated: standard, pass, 38 terms through n = 364 (2026-11-01)
 * Helpers:   build_primes_upto, ipow, ipow_idx, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"
#include "../utils/isqrt.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    if (n == 1) return seq_one();

    /* Keep a copy for factorisation */
    uint64_t nn = (uint64_t)n;
    seq_t result = seq_one();

    /* Generate primes up to sqrt(n) */
    idx_t limit = isqrt(nn);
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
            seq_t pp = ipow_idx((seq_t)p, e);
            /* part = pp^pp */
            seq_t part = ipow(pp, pp);
            /* Multiply into result */
            result = seq_mul(result, part);
        }
    }

    /* Handle remaining prime factor > sqrt(original n) */
    if (nn > 1) {
        seq_t pp = ipow_idx((seq_t)nn, 1);        /* pp = nn (prime power 1) */
        seq_t part = ipow(pp, pp);            /* part = pp^pp */
        result = seq_mul(result, part);
    }

    free(primes);
    return result;
}
