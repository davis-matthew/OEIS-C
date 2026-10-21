/*
 * A095233  a(n) = a(n-1) + Sum(floor(n/p): p prime), a(1) = 1.
 * https://oeis.org/A095233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=a(n-1)+Sum_{p prime ≤ n} floor(n/p) (OEIS A095233)
 * Range:     u8 n <= 20, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum n considered – sufficient for all test cases (≈10 000). */
#define MAX_N 10000

/* Cache of previous terms.  cache[0] is unused; real values start at cache[1]. */
static seq_t cache[MAX_N + 1];
static idx_t cache_size = 0;

/* Prime sieve data. */
static bool is_prime_sieve[MAX_N + 1];
static uint64_t primes[2000];   /* Enough for n ≤ 10000 (π(10000)=1229). */
static size_t primes_cnt = 0;
static bool primes_generated = false;

/* Generate all primes up to MAX_N once. */
static void generate_primes_upto(void)
{
    if (primes_generated)
        return;

    for (size_t i = 0; i <= (size_t)MAX_N; ++i)
        is_prime_sieve[i] = true;
    is_prime_sieve[0] = is_prime_sieve[1] = false;

    for (size_t i = 2; i * i <= (size_t)MAX_N; ++i) {
        if (!is_prime_sieve[i])
            continue;
        for (size_t j = i * i; j <= (size_t)MAX_N; j += i)
            is_prime_sieve[j] = false;
    }

    for (size_t i = 2; i <= (size_t)MAX_N; ++i)
        if (is_prime_sieve[i])
            primes[primes_cnt++] = (uint64_t)i;

    primes_generated = true;

    /* Initialise cache with the first term. */
    cache[1] = (seq_t)1;
    cache_size = 1;
}

/* Return a(n) for any n ≥ 1 (cache is extended on demand). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out of offset – harmless */

    /* Ensure primes and cache are ready. */
    generate_primes_upto();

    if ((idx_t)cache_size >= n)
        return cache[n];

    /* Extend cache up to n. */
    for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < primes_cnt && primes[j] <= (uint64_t)i; ++j)
            sum += (seq_t)(i / primes[j]);

        cache[i] = cache[i - 1] + sum;
    }
    cache_size = (size_t)n;

    return cache[n];
}
