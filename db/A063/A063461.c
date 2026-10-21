/*
 * A063461  Primes of the form k^2 + prime(k) + 1.
 * https://oeis.org/A063461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A063461 – primes of the form k^2 + prime(k) + 1
 *
 *  Sequence is defined as:
 *      a(n) = the n-th prime value of k^2 + prime(k) + 1,  k = 1,2,3,...
 *  The implementation uses a simple integer sieve to generate a list of
 *  primes up to a fixed limit.  The primality test for each candidate
 *  checks divisibility only by primes ≤ √candidate, which is fast for
 *  the required range (n ≤ 10000).  The sequence values and the prime
 *  table are cached statically so that calls to a() with arbitrary
 *  n are answered in O(n) time without recomputation.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------ */
/*  Global tables and counters                                 */
/* ------------------------------------------------------------ */
#define PRIME_LIMIT 5000000   /* prime sieve bound (≈ 350 000 primes) */
static uint64_t *prime_list = NULL;
static size_t     prime_count = 0;

static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/*  Keep track of the largest k examined so far. */
static size_t last_k = 0;

/* ------------------------------------------------------------ */
/*  Integer sieve (dynamic allocation)                         */
/* ------------------------------------------------------------ */
static void init_prime_sieve(void)
{
    if (prime_list != NULL)
        return;                          /* already initialised */

    /* Allocate boolean sieve: true = prime candidate */
    bool *sieve = (bool *)malloc(PRIME_LIMIT + 1);
    if (!sieve)
        return;                          /* allocation failure – unlikely */

    memset(sieve, true, PRIME_LIMIT + 1);
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++cnt;

    /* Allocate and fill prime list */
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) { free(sieve); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[idx++] = i;

    prime_count = cnt;
    free(sieve);
}

/* ------------------------------------------------------------ */
/*  Simple deterministic primality test (uses prime_list)      */
/* ------------------------------------------------------------ */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;

    /* Divide by primes <= sqrt(n) */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/*  Ensure the cache has room for `n` terms                    */
/* ------------------------------------------------------------ */
static void grow_cache(size_t needed)
{
    if (seq_cap >= needed)
        return;

    size_t newcap = (seq_cap == 0) ? 16 : seq_cap * 2;
    while (newcap < needed)
        newcap *= 2;

    seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* ------------------------------------------------------------ */
/*  a(idx_t n) – main sequence routine                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_prime_sieve();               /* primes up to PRIME_LIMIT */

    /* Generate until we have at least n terms */
    size_t k = last_k + 1;
    while (seq_len < (size_t)n) {
        if (k > prime_count) {
            /* In the unlikely event the pre‑sieved list is too small,
               increase the limit (not needed for n ≤ 10000). */
            break;
        }

        /* candidate = k^2 + prime(k) + 1   (k is 1‑based) */
        uint64_t pk = prime_list[k - 1];
        uint64_t cand = (uint64_t)k * k + pk + 1;

        if (is_prime_u64(cand)) {
            grow_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)cand;
        }
        ++k;
    }

    last_k = k - 1;
    return seq_cache[(size_t)n - 1];
}
