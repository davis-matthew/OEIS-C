/*
 * A023847  Sum of exponents in prime-power factorization of binomial(5n, 2n).
 * https://oeis.org/A023847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = bigOmega(binomial(5n, 2n)) where bigOmega(m) is the sum
 *            of exponents in the prime factorization of m
 * Range:     u8 n <= 492, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Legendre's formula: exponent of prime p in N! */
static seq_t legendre_exponent(idx_t N, uint64_t p)
{
    seq_t exp = 0;
    idx_t power = p;
    while (power <= N) {
        exp += N / power;
        power *= p;
    }
    return exp;
}

/* Generate all primes <= limit into a dynamic array.
   The caller must free the returned array. */
static uint64_t *generate_primes(idx_t limit, size_t *out_count)
{
    /* Simple sieve with a byte per number */
    bool *is_composite = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_composite) {
        *out_count = 0;
        return NULL;
    }
    memset(is_composite, 0, (size_t)(limit + 1) * sizeof(bool));

    size_t cap = 64;
    uint64_t *primes = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        *out_count = 0;
        return NULL;
    }
    size_t count = 0;

    for (idx_t num = 2; num <= limit; ++num) {
        if (!is_composite[(size_t)num]) {
            if (count == cap) {
                cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
                if (!tmp) {
                    free(is_composite);
                    free(primes);
                    *out_count = 0;
                    return NULL;
                }
                primes = tmp;
            }
            primes[count++] = (uint64_t)num;

            if (num * num <= limit) {
                for (idx_t j = num * num; j <= limit; j += num) {
                    is_composite[(size_t)j] = true;
                }
            }
        }
    }

    free(is_composite);
    *out_count = count;
    return primes;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t limit = 5 * n;
    size_t prime_count = 0;
    uint64_t *primes = generate_primes(limit, &prime_count);
    if (!primes) {
        return (seq_t)0;
    }

    seq_t sum = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        seq_t v5 = legendre_exponent(5 * n, p);
        seq_t v2 = legendre_exponent(2 * n, p);
        seq_t v3 = legendre_exponent(3 * n, p);
        sum += (v5 - v2 - v3);
    }

    free(primes);
    return sum;
}
