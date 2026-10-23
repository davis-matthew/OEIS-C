/*
 * A048199  Distance of primes to next odd multiple of 5 (where n mod 10 = 5).
 * https://oeis.org/A048199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5 - (p_n mod 10) + 10) mod 10 * where p_n is the n-th
 *            prime (n ≥ 1) and the result is the distance * from p_n to the
 *            next odd multiple of 5 (congruent to 5 mod 10)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for cached primes */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t prime_count = 0;         /* number of primes stored */

/* generate all primes up to 'limit' and store them in prime_cache */
static void generate_primes(size_t limit)
{
    /* allocate sieve array */
    bool *is_prime = (bool *)malloc(limit + 1);
    if (!is_prime) return;            /* allocation failed – unlikely */

    /* initialise */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* sieve of Eratosthenes */
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* allocate prime array */
    uint64_t *new_cache = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_cache[idx++] = i;

    /* replace old cache */
    free(prime_cache);
    prime_cache = new_cache;
    prime_count = cnt;

    free(is_prime);
}

/* ensure that prime_cache contains at least 'need' primes */
static void ensure_primes(idx_t need)
{
    if ((size_t)need <= prime_count) return;

    /* estimate a reasonable sieve limit; for n ≤ 10 000 a multiple of
       20 suffices (n=10 000 → 200 000 > 104 729). */
    size_t limit = (size_t)need * 20 + 100;

    /* regenerate primes until we have enough */
    do {
        generate_primes(limit);
        if (prime_count >= (size_t)need) break;
        limit *= 2;                 /* double the limit and try again */
    } while (1);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */

    ensure_primes(n);

    uint64_t p = prime_cache[n - 1];   /* nth prime, 1‑based index */
    uint64_t mod10 = p % 10;
    uint64_t offset = (5 + 10 - mod10) % 10;  /* distance to next odd 5 */
    return (seq_t)offset;
}
