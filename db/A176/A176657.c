/*
 * A176657  Numbers k such that prime(k) is not a twin prime and
 *          prime(prime(k)) is a twin prime.
 * https://oeis.org/A176657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that prime(k) is not a
 *            twin prime and prime(prime(k)) is a twin prime
 * Range:     u8 n <= 39, u16 n <= 9109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LIMIT 20000000U  /* upper bound for the sieve: primes up to LIMIT */

static bool *is_prime = NULL;          /* bitmap: is_prime[i] = 1 iff i is prime */
static uint32_t *prime_list = NULL;    /* array of primes in ascending order */
static size_t prime_count = 0;          /* number of primes generated */

static seq_t *cache = NULL;            /* memoised sequence values */
static size_t cache_used = 0;           /* number of terms stored in cache */
static size_t cache_cap = 0;            /* current capacity of cache */
static idx_t next_k = 1;                /* next candidate k to test */
static bool initialized = false;

/* Initialise sieve and prime list once */
static void init(void)
{
    if (initialized) return;

    /* allocate bitmap */
    is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes (odd only optimization could be added) */
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_count++;

    /* Build prime list */
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) abort();
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = i;

    /* Initialise cache */
    cache_cap = 128;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();

    initialized = true;
}

/* Check if a prime number p (assumed <= LIMIT) is a twin prime */
static bool is_twin_prime(uint32_t p)
{
    if (p < 3) return false; /* 2 is not a twin prime */
    if (p >= 3 && is_prime[p - 2]) return true;
    if ((uint64_t)p + 2 <= LIMIT && is_prime[p + 2]) return true;
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();
    if (n == 0) return 0;  /* undefined for n = 0 */

    while (cache_used < n) {
        if (next_k > prime_count) abort();  /* need more primes than precomputed */

        /* prime(k) */
        uint32_t pk = prime_list[next_k - 1];

        /* Condition 1: prime(k) is NOT a twin prime */
        if (!is_twin_prime(pk)) {
            /* Condition 2: prime(prime(k)) is a twin prime */
            if (pk <= prime_count) {  /* ensure pk is within the range of our list */
                uint32_t pk2 = prime_list[pk - 1];  /* prime of index pk */
                if (is_twin_prime(pk2)) {
                    /* Store k in cache */
                    if (cache_used == cache_cap) {
                        cache_cap <<= 1;
                        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        cache = tmp;
                    }
                    cache[cache_used++] = (seq_t)next_k;
                }
            }
        }
        next_k++;
    }
    return cache[n - 1];
}
