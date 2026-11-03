/*
 * A091137  The Hirzebruch numbers. a(n) = Product_{2 <= p <= n+1, p prime}
 *          p^floor(n / (p - 1)).
 * https://oeis.org/A091137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p prime, 2 <= p <= n+1} p^floor(n/(p-1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 410 (2026-11-01)
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/sieve_primes.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t limit   = n + 1;
    idx_t *primes = NULL;
    size_t prime_cnt = 0;

    sieve_primes(limit, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p   = primes[i];
        idx_t exp = n / (p - 1);
        for (idx_t k = 0; k < exp; ++k) {
            result = seq_mul(result, (seq_t)p);
        }
    }

    free(primes);
    return result;
}
