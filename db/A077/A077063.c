/*
 * A077063  Squarefree kernel of prime(n) - 1.
 * https://oeis.org/A077063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_upper_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = rad(prime(n) - 1)
 * where rad(m) is the product of the distinct prime factors of m.
 * The function uses a simple sieve to generate the first n primes on demand
 * and then factors prime(n)-1 by trial division with those primes.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "../utils/estimate_upper_for_n.h"

/* Static state for prime generation */
static uint32_t *prime_list = NULL;   /* prime_list[0] = 2, last element = prime_n */
static size_t    prime_count = 0;

/* Generate at least n primes. */
static void ensure_primes_for_n(size_t n)
{
    if (prime_count >= n) return;

    size_t limit = estimate_upper_for_n(n);

    char *is_comp = (char *)calloc(limit + 1, 1);
    if (!is_comp) abort();

    if (prime_list) free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_count == 0) {
                prime_list = (uint32_t *)malloc(32 * sizeof(uint32_t));
                if (!prime_list) abort();
            } else if (prime_count % 32 == 0) {
                prime_list = (uint32_t *)realloc(prime_list,
                                                (prime_count + 32) * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = (uint32_t)i;

            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
}

/* Compute the squarefree kernel (radical) of n. */
static seq_t rad_of(seq_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t res = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) {
            res *= p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) res *= n;   /* remaining prime factor */
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, offset is 1 */

    ensure_primes_for_n((size_t)n);

    uint32_t prime_n = prime_list[(size_t)(n - 1)];
    seq_t m = (seq_t)prime_n - 1;
    return rad_of(m);
}
