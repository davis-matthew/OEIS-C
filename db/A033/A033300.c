/*
 * A033300  Smallest safe prime ((p-1)/2 is also prime) > n-th prime.
 * https://oeis.org/A033300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest safe prime > n-th prime (0‑based n)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* internal state: sieve and prime list */
static bool *sieve        = NULL;   /* true if composite? false if prime */
static size_t sieve_size  = 0;      /* highest index stored in sieve */
static uint64_t *prime_list = NULL;
static size_t prime_cnt    = 0;      /* number of primes in prime_list */

/* enlarge sieve to cover at least up to 'limit' (inclusive) and rebuild the prime list */
static void ensure_sieve_upto(size_t limit)
{
    if (limit <= sieve_size) return;                /* already sufficient */

    /* allocate new sieve, true means potential prime initially */
    size_t new_sz = limit + 1;
    bool *new_sieve = (bool*)malloc(new_sz);
    for (size_t i = 0; i < new_sz; ++i) new_sieve[i] = true;
    new_sieve[0] = new_sieve[1] = false;

    for (size_t i = 2; i * i <= limit; ++i)
        if (new_sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                new_sieve[j] = false;
        }

    /* count primes and build the prime list */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (new_sieve[i]) ++cnt;
    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (new_sieve[i]) new_list[idx++] = i;

    /* replace old state */
    free(sieve);
    free(prime_list);
    sieve = new_sieve;
    sieve_size = limit;
    prime_list = new_list;
    prime_cnt = cnt;
}

/* ensure that we have at least the (n+1)-th prime (0‑based) */
static void ensure_prime_count(idx_t n)
{
    if (prime_cnt > (size_t)n) return;
    size_t limit = sieve_size > 0 ? sieve_size : 20;
    while (prime_cnt <= (size_t)n) {
        limit *= 2;
        ensure_sieve_upto(limit);
    }
}

/* check if a 64‑bit integer is prime using the current prime list / sieve */
static bool is_prime_u64(uint64_t x)
{
    if (x <= sieve_size) return sieve[x];
    /* extend sieve so that you guarantee primality test up to sqrt(x) at least */
    size_t ext = x;
    if (ext < 1000) ext = 1000;
    while (ext <= x && ext <= 100000000) ext *= 2;
    ensure_sieve_upto(ext);
    if (x <= sieve_size) return sieve[x];

    /* trial division by primes <= sqrt(x) */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= x; ++i)
        if (x % prime_list[i] == 0) return false;
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* obtain the n-th prime (0‑based) */
    ensure_prime_count(n);
    uint64_t nth_prime = prime_list[(size_t)n];
    uint64_t candidate = nth_prime + 1;

    /* search for the smallest safe prime > nth_prime */
    while (true) {
        /* candidate must be odd (except 2) */
        if (candidate % 2 == 0) { ++candidate; continue; }

        /* ensure we can test primality of candidate and (candidate-1)/2 */
        if (candidate > sieve_size) ensure_sieve_upto(candidate);
        uint64_t half = (candidate - 1) / 2;
        if (half > sieve_size) ensure_sieve_upto(half);

        if (sieve[candidate] && sieve[half]) return (seq_t)candidate;
        ++candidate;
    }
}
