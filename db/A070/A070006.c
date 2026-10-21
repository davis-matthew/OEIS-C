/*
 * A070006  Composite numbers that are not a prime power and whose distinct
 *          prime divisors' arithmetic mean is a prime.
 * https://oeis.org/A070006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A070006:
   Composite numbers that are not a prime power and whose distinct
   prime divisors' arithmetic mean is a prime. */
/* The sequence is produced by a simple search:
   test successive integers, keep those that satisfy the above property.
   The result is cached, so any n can be requested in O(1) time after
   O(n) precomputation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- prime table ---------------------------------------------------- */
#define PRIME_LIMIT 2000000      /* enough for factorising the searched numbers */

static uint32_t *primes = NULL;   /* array of primes */
static size_t   primes_cnt = 0;   /* number of primes stored */

/* initialise the prime table once */
static void init_primes(void)
{
    if (primes != NULL)
        return;                  /* already initialised */

    /* Allocate a bitset for the sieve */
    bool *is_composite = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_composite)
        abort();

    size_t capacity = 64;
    primes = (uint32_t *)malloc(capacity * sizeof(uint32_t));
    if (!primes)
        abort();

    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            /* store prime */
            if (primes_cnt == capacity) {
                capacity <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, capacity * sizeof(uint32_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;

            /* mark multiples */
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int64_t j = (int64_t)i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    free(is_composite);
}

/* ---- primality test used for the arithmetic mean ----------------- */
static bool is_prime_fast(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    init_primes();
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* ---- test whether a number satisfies the definition ---------------- */
static bool qualifies(uint64_t n)
{
    if (n < 4)               /* must be composite */
        return false;

    uint64_t temp = n;
    uint64_t sum_distinct = 0;
    size_t  distinct_cnt = 0;

    /* factorise using the precomputed primes */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (temp % p == 0) {
            ++distinct_cnt;
            sum_distinct += p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) {          /* remaining prime factor > sqrt(original) */
        ++distinct_cnt;
        sum_distinct += temp;
    }

    if (distinct_cnt < 2)    /* prime power or only one prime */
        return false;

    if (sum_distinct % distinct_cnt != 0)
        return false;

    uint64_t mean = sum_distinct / distinct_cnt;
    return is_prime_fast(mean);
}

/* ---- static cache for the sequence --------------------------------- */
#define CACHE_MAX 20000       /* safe upper bound for n up to 10000 */
static seq_t seq_cache[CACHE_MAX];   /* array of cached terms */
static size_t cache_len = 0;         /* how many terms are currently cached */
static uint64_t next_to_test = 2;    /* candidate to be examined next */

/* ---- the required function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for indices < 1 */
        return 0;

    init_primes();

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++next_to_test;
        if (qualifies(next_to_test)) {
            /* store the term in the cache */
            if (cache_len == CACHE_MAX)   /* safety guard – should not happen */
                abort();
            seq_cache[cache_len++] = (seq_t)next_to_test;
        }
    }
    return seq_cache[(size_t)n - 1];
}
