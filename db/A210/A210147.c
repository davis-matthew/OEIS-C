/*
 * A210147  Numbers expressible as 2*p+q, p and q distinct primes.
 * https://oeis.org/A210147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210147 – numbers expressible as 2*p + q with
 * p and q distinct primes.  The function generates terms by
 * testing each integer in ascending order: for each x it
 * checks all primes p ≤ (x-2)/2 and then verifies whether
 * q = x-2p is prime (and q ≠ p).  Results are cached
 * to allow arbitrary-access calls. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---  Prime sieve and list ------------------------------------------ */
static uint8_t  *prime_flags  = NULL;   /* 0/1 flag array, allocated up to sieve_limit */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t    prime_count  = 0;
static uint64_t  sieve_limit  = 0;      /* highest number currently sieved */

/* Build a sieve up to 'limit' (inclusive) and populate prime_list. */
static void sieve_up_to(uint64_t limit)
{
    /* Allocate flag array 0/1 */
    uint8_t *new_flags = (uint8_t *)malloc(limit + 1);
    if (!new_flags) exit(1);
    memset(new_flags, 1, limit + 1);
    if (limit >= 0) new_flags[0] = 0;
    if (limit >= 1) new_flags[1] = 0;

    /* Classic Eratosthenes */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (new_flags[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                new_flags[j] = 0;
            }
        }
    }

    /* Count primes and allocate prime list */
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_flags[i]) ++cnt;
    }

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) exit(1);

    uint64_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_flags[i]) new_list[pos++] = i;
    }

    /* Replace old data */
    free(prime_flags);
    free(prime_list);

    prime_flags = new_flags;
    prime_list  = new_list;
    prime_count = (size_t)cnt;
    sieve_limit = limit;
}

/* Ensure we have a sieve covering at least 'limit'. */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;
    sieve_up_to(limit);
}

/* ---  Cache for already generated terms ----------------------------- */
static seq_t *cache       = NULL;     /* array of produced terms */
static size_t cache_len   = 0;        /* number of terms in cache   */
static size_t cache_cap   = 0;        /* allocated capacity */

/* Append a term to the cache, growing the array as needed. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_mem) exit(1);
        cache = new_mem;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---  Main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n<1 is irrelevant */

    /* Initialise sieve lazily */
    if (sieve_limit == 0) {
        sieve_up_to(200000);               /* starting bound, > initial terms */
    }

    /* Keep generating until we have at least n terms */
    static uint64_t last_checked = 6;      /* last integer examined before 7 */
    while (cache_len < (size_t)n) {
        ++last_checked;
        uint64_t x = last_checked;

        /* Make sure all numbers up to x can be checked for primality */
        ensure_sieve(x);

        /* Find distinct primes p, q with x = 2*p + q */
        bool ok = false;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > (x - 2) / 2) break;              /* 2p would exceed x */
            uint64_t q = x - 2 * p;
            if (q == p) continue;                   /* distinctness check */
            /* q is <= x, so prime_flags has been sieved up to x */
            if (q <= sieve_limit && prime_flags[q]) {
                ok = true;
                break;
            }
        }

        if (ok) {
            cache_append((seq_t)x);
        }
    }

    return cache[(size_t)n - 1];
}
