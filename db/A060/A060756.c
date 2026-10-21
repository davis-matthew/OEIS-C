/*
 * A060756  a(n) is the smallest number for which exactly n primes are bounded
 *          between a(n) and 2a(n) exclusively.
 * https://oeis.org/A060756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 5708, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest m with exactly n primes in the open interval (m,2m).
   count_primes_between(m) = π(2m-1) - π(m).
   Uses a pre‑computed prime sieve up to a fixed LIMIT. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static const int LIMIT = 3000000;          /* must satisfy 2*max_a < LIMIT */

static bool sieve_done = false;
static unsigned char *is_prime;
static int *prime_prefix;                   /* π[i] stored as int */

/* Build sieve and prefix array once */
static void init_sieve(void)
{
    if (sieve_done) return;
    is_prime = (unsigned char*)calloc(LIMIT + 1, sizeof(unsigned char));
    prime_prefix = (int*)malloc((LIMIT + 1) * sizeof(int));

    for (int i = 2; i <= LIMIT; ++i) is_prime[i] = 1;
    for (int p = 2; (int64_t)p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (int64_t q = (int64_t)p * p; q <= LIMIT; q += p)
                is_prime[q] = 0;
        }
    }
    prime_prefix[0] = 0;
    for (int i = 1; i <= LIMIT; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + is_prime[i];
    }
    sieve_done = true;
}

/* Count primes in the open interval (m,2m).  Assumes 2m-1 ≤ LIMIT. */
static inline int count_primes_between(idx_t m)
{
    int low  = (int)m;
    int high = 2 * low - 1;
    return prime_prefix[high] - prime_prefix[low];
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0..cached_len-1] = a(0..cached_len-1) */
    static size_t cached_len = 0; /* number of cached terms */
    static size_t cache_cap = 0;  /* capacity of cache */
    static idx_t next_m = 1;      /* next m to test */

    init_sieve();

    /* Grow cache if necessary */
    while (cached_len <= n) {
        int cnt = count_primes_between(next_m);
        if ((idx_t)cnt == cached_len) {
            /* enlarge cache if needed */
            if (cached_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1);       /* allocation failure */
                cache    = new_cache;
                cache_cap = new_cap;
            }
            cache[cached_len] = (seq_t)next_m;
            cached_len++;
        }
        next_m++;
    }

    return cache[n];
}
