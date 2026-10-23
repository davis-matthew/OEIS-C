/*
 * A126107  Primes p such that 2*p+1 and 2*p+3 are twin primes.
 * https://oeis.org/A126107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Not a closed form; compute by searching for primes p such that *
 *            2p+1 and 2p+3 are twin primes
 * Range:     u8 n <= 7, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */

/* ---------- Prime sieve utilities ---------- */
static bool *comp = NULL;          /* composite flag array */
static uint32_t *prime_list = NULL;/* list of primes up to sieve_limit */
static size_t prime_cnt = 0;       /* number of primes found */
static uint64_t sieve_limit = 0;   /* uppermost number sieved */

/* Build a simple sieve up to 'limit' (inclusive).  Existing data is freed. */
static void build_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;          /* already enough */

    /* free previous allocation */
    free(comp);
    free(prime_list);

    /* allocate flags */
    comp = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!comp) abort();
    memset(comp, 0, (size_t)(limit + 1));
    comp[0] = comp[1] = 1;

    /* sieve */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                comp[j] = 1;
        }
    }

    /* count primes and allocate list */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) abort();

    uint64_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!comp[i])
            prime_list[idx++] = (uint32_t)i;

    prime_cnt   = cnt;
    sieve_limit = limit;
}

/* primality lookup using the sieve when possible */
static inline bool is_prime_lookup(uint64_t x)
{
    if (x <= sieve_limit)
        return !comp[x];
    return is_prime(x);
}

/* ---------- Sequence generator ---------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n==0 is unused */
        return 0;

    /* initial guess for sieve limit */
    uint64_t limit = sieve_limit;
    if (limit == 0)
        limit = 5000000;          /* enough for the first several thousand terms */

    while (1) {
        if (limit > sieve_limit)
            build_sieve(limit);

        size_t count = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p   = prime_list[i];
            uint64_t n1  = 2 * p + 1;
            uint64_t n3  = 2 * p + 3;

            if (is_prime_lookup(n1) && is_prime_lookup(n3)) {
                ++count;
                if (count == (size_t)n)
                    return (seq_t)p;
            }
        }

        /* not found yet – enlarge the sieve */
        limit = limit * 2 + 1000;
    }
}
