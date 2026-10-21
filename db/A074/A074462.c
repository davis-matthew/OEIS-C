/*
 * A074462  Average digit (rounded up) in the decimal expansion of prime(n).
 * https://oeis.org/A074462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = ⌈ digit_sum(prime(n)) / digit_count(prime(n)) ⌉
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The routine generates the first ‘n’ primes on demand (caching them in
    a static array) and then computes the digit sum and digit count of the
    n‑th prime.  The result is the ceiling of the average digit value. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime cache ------------------------------------------------- */
static uint64_t *prime_cache = NULL;     /* array of primes */
static size_t   prime_cache_cap = 0;     /* capacity of the array */
static size_t   prime_cache_len = 0;     /* number of primes stored */

/* grow the prime cache to hold at least ‘min_cap’ primes */
static void prime_cache_grow(size_t min_cap)
{
    if (prime_cache_cap >= min_cap) return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                     /* unrecoverable allocation error */
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

/* simple primality test using the primes already in the cache */
static bool is_prime_candidate(uint64_t cand)
{
    /* test divisibility by cached primes up to sqrt(cand) */
    for (size_t i = 0; i < prime_cache_len; ++i) {
        uint64_t p = prime_cache[i];
        if ((unsigned __int128)p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* generate primes until at least ‘n’ primes are cached */
static void generate_primes_to(idx_t n)
{
    prime_cache_grow(n);
    if (prime_cache_len >= n) return;       /* we already have enough */

    if (prime_cache_len == 0) {              /* start with 2 */
        prime_cache[0] = 2;
        prime_cache_len = 1;
    }

    uint64_t candidate = prime_cache[prime_cache_len - 1] + 1;
    while (prime_cache_len < n) {
        if (is_prime_candidate(candidate))
            prime_cache[prime_cache_len++] = candidate;
        ++candidate;
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset is 1; undefined below it */

    generate_primes_to(n);

    uint64_t p = prime_cache[n - 1];

    /* compute digit sum and digit count of the prime */
    seq_t sum   = 0;
    seq_t count = 0;
    uint64_t t  = p;
    while (t > 0) {
        sum   += t % 10;
        t     /= 10;
        ++count;
    }

    /* ceiling division: (sum + count - 1) / count */
    seq_t den = count;                      /* cast to seq_t as required */
    return (sum + den - 1) / den;
}
