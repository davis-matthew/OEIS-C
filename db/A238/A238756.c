/*
 * A238756  Number of ordered ways to write n = k + m (k > 0 and m > 0) such
 *          that 2*k + 1, prime(prime(k)) - prime(k) + 1 and prime(prime(m)) -
 *          prime(m) + 1 are all prime.
 * https://oeis.org/A238756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (k, m) with k + m = n, k > 0, m >
 *            0, such that * 2*k + 1 is prime, * prime(prime(k)) – prime(k) +
 *            1 is prime, * prime(prime(m)) – prime(m) + 1 is prime. (No
 *            condition on 2*m + 1.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/*  Sieve of Eratosthenes --------------------------------------------------- */
static const idx_t SIEVE_LIMIT = 4000000;   /* large enough for n ≤ 10000 */

/* prime_is[i] == true  iff i is prime */
static bool   *prime_is   = NULL;
/* list of primes, 1‑based: prime_list[0] = 2, prime_list[1] = 3, ... */
static uint64_t *prime_list = NULL;
static idx_t    prime_count = 0;
static bool     sieve_done  = false;

static void init_sieve(void)
{
    if (sieve_done) return;

    /* Allocate boolean array and mark all as potential primes */
    prime_is = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!prime_is) exit(EXIT_FAILURE);
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) prime_is[i] = true;

    /* Classic sieve */
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (prime_is[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_is[j] = false;

    /* Build prime list */
    size_t capacity = SIEVE_LIMIT / 10 + 1;   /* initial guess */
    prime_list = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);
    prime_count = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_is[i]) {
            if (prime_count == capacity) {
                capacity <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, capacity * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint64_t)i;
        }

    sieve_done = true;
}

/* -------------------------------------------------------------------------- */
/*  Memoisation of a(n) ------------------------------------------------------ */
static seq_t *cache      = NULL;   /* cache[n] holds a(n) for n < cache_cap */
static bool  *cached_arr = NULL;   /* cached_arr[n] == true if cache[n] valid */
static size_t cache_cap  = 0;

/* -------------------------------------------------------------------------- */
/*  Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Prepare sieve on first call */
    if (!sieve_done) init_sieve();

    /* Very small offset: a(1) = 0 by definition */
    if (n == (idx_t)1) return (seq_t)0;

    /* Increase cache size if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        bool *new_cached = (bool *)realloc(cached_arr, new_cap * sizeof(bool));
        if (!new_cache || !new_cached) exit(EXIT_FAILURE);

        /* Valid new entries start as 0 and false */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            new_cache[i] = 0;
            new_cached[i] = false;
        }

        cache      = new_cache;
        cached_arr = new_cached;
        cache_cap  = new_cap;
    }

    if (cached_arr[n]) return cache[n];

    /* Compute a(n) */
    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t m = n - k;

        /* ---------- Check k ----------
         * 1. 2*k + 1 must be prime
         * 2. prime(prime(k)) - prime(k) + 1 must be prime
         */
        if (!prime_is[2 * k + 1]) continue;

        /* prime(k) */
        uint64_t pk = prime_list[k - 1];
        if (pk > (uint64_t)prime_count) continue;  /* safety */
        uint64_t qk = prime_list[pk - 1];          /* prime(prime(k)) */
        uint64_t val_k = qk - pk + 1;
        if (val_k > SIEVE_LIMIT || !prime_is[val_k]) continue;

        /* ---------- Check m ----------
         * 1. prime(prime(m)) - prime(m) + 1 must be prime
         *    (no requirement on 2*m + 1)
         */
        uint64_t pm = prime_list[m - 1];
        if (pm > (uint64_t)prime_count) continue;
        uint64_t qm = prime_list[pm - 1];          /* prime(prime(m)) */
        uint64_t val_m = qm - pm + 1;
        if (val_m > SIEVE_LIMIT || !prime_is[val_m]) continue;

        /* All conditions satisfied */
        count += (seq_t)1;
    }

    cache[n]      = count;
    cached_arr[n] = true;
    return count;
}
