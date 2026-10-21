/*
 * A064775  Number of positive integers k <= n such that all prime divisors of
 *          k are <= sqrt(k).
 * https://oeis.org/A064775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - Σ_{p≤√n}(p-1) - Σ_{√n<p≤n} ⌊n/p⌋ (p prime)
 * Range:     u8 n <= 890, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/isqrt_ull.h"

/* a(n) -- number of integers k ≤ n with all prime divisors ≤ √k */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    /* allocate a boolean array for primality */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(N + 1));
    if (!is_prime) return 0;                    /* out of memory */
    memset(is_prime, 1, (size_t)(N + 1));
    if (N >= 0) is_prime[0] = 0;
    if (N >= 1) is_prime[1] = 0;

    /* simple sieve up to N */
    for (uint64_t i = 2; i * i <= N; ++i) {
        if (is_prime[(size_t)i]) {
            for (uint64_t j = i * i; j <= N; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    uint64_t sqrt_n = isqrt_ull(N);
    seq_t result = (seq_t)N;

    /* subtract (p-1) for primes ≤ √n */
    for (uint64_t p = 2; p <= sqrt_n; ++p) {
        if (is_prime[(size_t)p])
            result -= (seq_t)(p - 1);
    }

    /* subtract ⌊n/p⌋ for primes √n < p ≤ n */
    for (uint64_t p = sqrt_n + 1; p <= N; ++p) {
        if (is_prime[(size_t)p])
            result -= (seq_t)(N / p);
    }

    free(is_prime);
    return result;
}
