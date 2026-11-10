/*
 * A203704  v(n+1)/v(n), where v=A203703.
 * https://oeis.org/A203704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (p_i^2 + p_i*p_{n+1} + p_{n+1}^2), p_k =
 *            k-th prime; equals v(n+1)/v(n) with v = A203703
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n + 1, &primes, &cnt);
    if (cnt != (size_t)(n + 1)) {
        free(primes);
        return 0;
    }
    seq_t q = (seq_t)primes[n];
    seq_t result = 1;
    for (size_t i = 0; i < (size_t)n; ++i) {
        seq_t p = (seq_t)primes[i];
        result *= (p * p + p * q + q * q);
    }
    free(primes);
    return result;
}
