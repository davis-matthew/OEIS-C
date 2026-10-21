/*
 * A240657  Least k such that 2^k == -1 (mod prime(n)), or 0 if no such k
 *          exists.
 * https://oeis.org/A240657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014664(n)/2 if A014664(n) is even, otherwise 0 where
 *            A014664(n) is the multiplicative order of 2 modulo prime(n)
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   powmod_2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod_2_uint64.h"

/* --- Prime generation -------------------------------------------------- */
/* Static cache of primes used for the sequence. */
static uint64_t *prime_cache         = NULL;   /* array of primes */
static size_t    prime_cache_count   = 0;      /* number of stored primes   */
static idx_t     prime_cache_limit   = 0;      /* maximum prime generated   */

/* Simple Eratosthenes sieve to generate all primes up to 'limit'.
   The function overwrites the current cache with new primes. */
static void generate_primes_up_to(idx_t limit)
{
    /* Allocate sieve */
    size_t n = (size_t)limit + 1;
    unsigned char *sieve = (unsigned char *)calloc(n, 1);
    if (!sieve) return;                /* allocation failed – leave cache empty */

    /* Sieve */
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i) sieve[j] = 1;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) if (!sieve[i]) ++count;

    /* Allocate new cache */
    uint64_t *new_primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!new_primes) {
        free(sieve);
        return;                       /* allocation failed – leave cache unchanged */
    }

    /* Fill cache */
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (!sieve[i]) new_primes[idx++] = (uint64_t)i;

    /* Replace old cache */
    free(prime_cache);
    prime_cache         = new_primes;
    prime_cache_count   = count;
    prime_cache_limit   = limit;

    free(sieve);
}

/* Ensure that at least 'need' primes are cached. */
static void ensure_primes_for(size_t need)
{
    if (prime_cache_count >= need) return;

    /* Rough upper bound for the nth prime: n*(log n + log log n) < 30*n for our range.
       For n up to 10000 we use a safe constant factor. */
    idx_t new_limit = (prime_cache_limit == 0) ? 200 : 30 * (idx_t)need;
    if (new_limit < 200) new_limit = 200;          /* small initial limit */

    /* Expand until we have enough primes */
    while (prime_cache_count < need) {
        generate_primes_up_to(new_limit);
        if (prime_cache_count >= need) break;
        new_limit *= 2;                            /* double and try again */
    }
}

/* --- Sequence implementation ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* sequence is 1‑based */

    /* Make sure we have the n‑th prime cached */
    ensure_primes_for((size_t)n);
    if (prime_cache_count < (size_t)n)
        return (seq_t)0;              /* safety fallback – should not happen */

    uint64_t p = prime_cache[(size_t)n - 1];   /* nth prime, 0‑based indexing */

    /* Special case p==2: no k exists because 2^k ≡ 0 (mod 2) ≠ -1 (≡1). */
    if (p == 2) return (seq_t)0;

    uint64_t order = p - 1;          /* multiplicative order of 2 divides p-1 */

    /* Compute distinct prime factors of 'order' using the cached primes. */
    uint64_t factors[32];
    size_t   factor_cnt = 0;
    uint64_t temp = order;
    for (size_t i = 0; i < prime_cache_count && prime_cache[i] * prime_cache[i] <= temp; ++i) {
        uint64_t q = prime_cache[i];
        if ((temp % q) == 0) {
            factors[factor_cnt++] = q;
            while ((temp % q) == 0) temp /= q;
            if (factor_cnt == sizeof(factors) / sizeof(factors[0])) break; /* safety */
        }
    }
    if (temp > 1 && factor_cnt < sizeof(factors) / sizeof(factors[0]))
        factors[factor_cnt++] = temp;

    /* Reduce 'order' by testing each prime factor. */
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t q = factors[i];
        while ((order % q) == 0) {
            uint64_t candidate = order / q;
            if (powmod_2_uint64(candidate, p) == (seq_t)1)
                order = candidate;
            else
                break;
        }
    }

    /* If the minimal order is even, k = order/2; otherwise no such k. */
    if ((order & 1ULL) == 0ULL)
        return (seq_t)(order >> 1);
    else
        return (seq_t)0;
}
