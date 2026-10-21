/*
 * A053023  Divisor function applied thrice to n!.
 * https://oeis.org/A053023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(d(d(n!)))
 * Range:     u8 n <= 285, u16 n <= 6193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/add_factors.h"

/* Simple sieve that returns all primes up to 'limit' (inclusive). */
static void simple_sieve(uint64_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    uint64_t size = limit + 1;
    char *is_prime = (char *)calloc(size, sizeof(char));
    if (!is_prime) exit(1);
    for (uint64_t i = 2; i < size; ++i) is_prime[i] = 1;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }
    size_t pcnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++pcnt;
    *primes_out = (uint32_t *)malloc(pcnt * sizeof(uint32_t));
    *cnt_out = pcnt;
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) (*primes_out)[idx++] = (uint32_t)i;
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint64_t limit = (uint64_t)n + 1;           /* primes up to n+1 suffice */
    uint32_t *prime_list = NULL;
    size_t prime_count = 0;
    simple_sieve(limit, &prime_list, &prime_count);

    /* prime value -> index mapping */
    int *prime_idx = (int *)calloc(limit + 1, sizeof(int));
    for (size_t i = 0; i < prime_count; ++i) {
        prime_idx[prime_list[i]] = (int)i;
    }

    uint64_t *f = (uint64_t *)calloc(prime_count, sizeof(uint64_t));
    uint64_t *g = (uint64_t *)calloc(prime_count, sizeof(uint64_t));

    /* compute exponents for d(n!) */
    for (size_t i = 0; i < prime_count && prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        uint64_t t = n, e = 0;
        while (t) { t /= p; e += t; }
        uint64_t val = e + 1;                 /* factor this number */
        add_factors(val, prime_list, prime_count, f, prime_idx);
    }

    /* compute exponents for d(d(n!)) */
    for (size_t i = 0; i < prime_count; ++i) {
        if (f[i] == 0) continue;
        uint64_t val = f[i] + 1;
        add_factors(val, prime_list, prime_count, g, prime_idx);
    }

    /* final result: product over (g[i] + 1) */
    seq_t result = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        if (g[i] == 0) continue;
        result *= (seq_t)(g[i] + 1);
    }

    free(prime_idx);
    free(f);
    free(g);
    free(prime_list);

    return result;
}
