/*
 * A085252  Number of ways to write n as sum of two powerful numbers
 *          (A001694).
 * https://oeis.org/A085252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of powerful numbers (A001694)
 *            whose sum equals n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Comparison function for qsort on uint64_t values */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Global static state */
static uint64_t *powers = NULL;
static size_t powers_cap = 0, powers_len = 0;
static uint64_t *primes = NULL;
static size_t primes_cap = 0, primes_len = 0;
static uint64_t current_limit = 0;

/* Recursive generator of powerful numbers */
static void generate_powerfuls(size_t idx, uint64_t cur, uint64_t limit)
{
    if (idx >= primes_len) return;

    uint64_t p = primes[idx];
    uint64_t val = cur;

    /* Generate factors with exponent e >= 2 */
    for (int e = 1; ; ++e) {
        /* Multiply by one more prime factor */
        if (val > limit / p) break;
        val *= p;

        /* Record once exponent reaches 2 */
        if (e >= 2) {
            /* Store the powerful number */
            if (powers_len == powers_cap) {
                powers_cap = powers_cap ? powers_cap * 2 : 16;
                powers = (uint64_t *)realloc(powers,
                                             powers_cap * sizeof(uint64_t));
            }
            powers[powers_len++] = val;

            /* Recurse to combine with the next prime */
            generate_powerfuls(idx + 1, val, limit);
        }
    }

    /* Skip this prime and try later primes with cur unchanged */
    generate_powerfuls(idx + 1, cur, limit);
}

/* Precompute powerful numbers up to n (inclusive) */
static void precompute_powerfuls(uint64_t limit)
{
    /* Free old data */
    free(powers);  powers = NULL; powers_cap = powers_len = 0;
    free(primes);  primes = NULL; primes_cap = primes_len = 0;

    /* Sieve primes up to sqrt(limit) */
    uint64_t prime_limit = (uint64_t)floor(sqrt((long double)limit)) + 1;
    uint8_t *is_prime = (uint8_t *)calloc(prime_limit + 1, sizeof(uint8_t));
    if (!is_prime) return;
    for (uint64_t i = 2; i <= prime_limit; ++i) is_prime[i] = 1;
    for (uint64_t i = 2; i * i <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= prime_limit; j += i)
                is_prime[j] = 0;
        }
    }
    for (uint64_t i = 2; i <= prime_limit; ++i) {
        if (is_prime[i]) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t *)realloc(primes,
                                             primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = i;
        }
    }
    free(is_prime);

    /* Add the number 1: it is powerful */
    if (powers_len == powers_cap) {
        powers_cap = powers_cap ? powers_cap * 2 : 16;
        powers = (uint64_t *)realloc(powers,
                                     powers_cap * sizeof(uint64_t));
    }
    powers[powers_len++] = 1;

    /* Generate all powerful numbers <= limit */
    generate_powerfuls(0, 1, limit);

    /* Sort the list for two‑pointer counting */
    qsort(powers, powers_len, sizeof(uint64_t), cmp_uint64);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Ensure we have all powerful numbers up to n */
    if ((uint64_t)n > current_limit) {
        current_limit = (uint64_t)n;
        precompute_powerfuls((uint64_t)n);
    }

    seq_t count = 0;
    size_t l = 0, r = powers_len ? powers_len - 1 : 0;

    while (l <= r) {
        uint64_t sum = powers[l] + powers[r];
        if (sum == (uint64_t)n) {
            ++count;
            if (l == r) break;    /* last possible pair */
            ++l;
            --r;
        } else if (sum < (uint64_t)n) {
            ++l;
        } else {
            if (r == 0) break;
            --r;
        }
    }

    return count;
}
