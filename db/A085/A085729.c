/*
 * A085729  Sum of prime factors of prime powers.
 * https://oeis.org/A085729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001414(A000961(n)) * A000961(n) is the nth prime power
 *            (1 included), and A001414(m) is the * sum of the prime factors
 *            of m (with multiplicities). The function * generates the
 *            required prime powers on demand and returns the sum * of prime
 *            factors of the nth prime power
 * Range:     u8 n <= 71, u16 n <= 6636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
/* Utility functions provided by the build system */
#include "../utils/build_primes_upto.h"
#include "../utils/_smallest_prime_factor.h"

/* Comparator for qsort on uint64_t values */
static int cmp_u64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x > *y) - (*x < *y);
}

/* Cached prime powers array */
static uint64_t *prime_powers_u64 = NULL;
static size_t prime_powers_len   = 0;

/* Generate at least target_n prime powers (1 inclusive). */
static void generate_prime_powers(idx_t target_n)
{
    if (prime_powers_len >= (size_t)target_n)
        return; /* already have enough */

    /* discard old cache */
    if (prime_powers_u64 != NULL) {
        free(prime_powers_u64);
        prime_powers_u64 = NULL;
        prime_powers_len = 0;
    }

    idx_t limit = 2; /* initial bound for primes */
    while (1) {
        /* generate primes up to current limit */
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes_upto(limit, &primes, &prime_cnt);

        /* estimate capacity: primes + prime powers up to limit */
        size_t cap = prime_cnt * 10 + 10;
        uint64_t *tmp = (uint64_t *)malloc(cap * sizeof(uint64_t));
        size_t cnt = 0;

        /* 1 is treated as the zeroth prime power */
        tmp[cnt++] = 1ULL;

        /* for each prime generate all powers <= limit */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t pow = p;
            while (pow <= (uint64_t)limit) {
                if (cnt >= cap) {
                    cap *= 2;
                    tmp = (uint64_t *)realloc(tmp, cap * sizeof(uint64_t));
                }
                tmp[cnt++] = pow;
                if (pow > (uint64_t)limit / p)
                    break;
                pow *= p;
            }
        }

        /* sort the powers */
        qsort(tmp, cnt, sizeof(uint64_t), cmp_u64);

        /* if we have enough, keep the list */
        if (cnt >= (size_t)target_n) {
            prime_powers_u64 = tmp;
            prime_powers_len = cnt;
            free(primes);
            break;
        }

        /* otherwise discard and try with larger limit */
        free(tmp);
        free(primes);
        limit *= 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* safety for indices below the offset */

    /* ensure we have at least n prime powers */
    generate_prime_powers(n);

    /* 0‑based index into the cached array */
    uint64_t m = prime_powers_u64[(size_t)(n - 1)];

    if (m == 1)
        return 0; /* sum of prime factors of 1 is 0 */

    /* find the base prime p and exponent e of m = p^e */
    uint64_t p = _smallest_prime_factor(m);
    size_t e = 0;
    uint64_t tmp = m;
    while (tmp > 1) {
        tmp /= p;
        ++e;
    }

    /* sum of prime factors of p^e is e * p */
    seq_t result = (seq_t)e * (seq_t)p;
    return result;
}
