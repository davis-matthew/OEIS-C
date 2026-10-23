/*
 * A014220  Next prime after n^3.
 * https://oeis.org/A014220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 991, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A014220.
   a(n) = the smallest prime greater than n³. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

/* Lazy prime table for trial division */
static uint32_t *prime_table = NULL;
static size_t prime_count  = 0;

/* Build primes up to 1 000 000 */
static void init_prime_table(void)
{
    if (prime_table != NULL)
        return;

    const unsigned int limit = 1000000;
    bool *sieve = (bool *)malloc(limit + 1);
    if (!sieve)
        exit(1); /* insufficient memory */
    memset(sieve, 0, limit + 1);

    /* Rough estimate for number of primes ≤ limit: ~limit/ln(limit) */
    size_t est = limit / 10;
    prime_table = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!prime_table) {
        free(sieve);
        exit(1);
    }

    size_t cnt = 0;
    for (unsigned int i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            prime_table[cnt++] = i;
            if ((unsigned long long)i * i <= limit) {
                for (unsigned int j = i * i; j <= limit; j += i)
                    sieve[j] = true;
            }
        }
    }

    prime_count = cnt;
    free(sieve);
}

/* Deterministic primality test for seq_t values using trial division
   with the cached prime table. */
static bool is_prime_small(seq_t x)
{
    if (x < 2)
        return false;
    if (x % 2 == 0)
        return x == 2;

    init_prime_table();

    seq_t limit = floor_sqrt(x);
    for (size_t i = 0; i < prime_count && (seq_t)prime_table[i] <= limit; ++i) {
        if (x % (seq_t)prime_table[i] == 0)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute n³ in a 128‑bit intermediate to avoid overflow when
       seq_t is 64‑bits or larger. */
    unsigned __int128 cube128 = (unsigned __int128)n *
                                (unsigned __int128)n *
                                (unsigned __int128)n;
    seq_t cube = (seq_t)cube128;

    /* Search for the next prime greater than n³ */
    seq_t candidate = cube + 1;
    while (!is_prime_small(candidate))
        ++candidate;

    return candidate;
}
