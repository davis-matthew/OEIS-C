/*
 * A156830  a(1) = 2. For n >= 2, a(n) = the smallest prime > a(n-1) + p(n) -
 *          p(n-1), where p(n) is the n-th prime.
 * https://oeis.org/A156830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 2,
 * a(n) = the smallest prime > a(n-1) + p(n) - p(n-1)   (n ≥ 2)
 * where p(n) is the n‑th prime.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* --------------------------------------------------------------------- */
/*  Prime generation (sieve of Eratosthenes)                            */
/* --------------------------------------------------------------------- */

#define PRIME_LIMIT 200000           /* enough for n ≤ 10 000 */
#define MAX_N       10000            /* maximum n we expect to query */

static uint8_t sieve[PRIME_LIMIT + 1];   /* 0 = prime, 1 = composite */
static uint64_t primes[MAX_N];           /* first MAX_N primes */
static size_t primes_count = 0;          /* actual number of primes found */

/* One‑time prime sieve */
static void init_primes(void)
{
    if (primes_count != 0) return;          /* already initialised */

    /* basic sieve */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) sieve[i] = 0;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = 1;

    /* collect first MAX_N primes */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT && idx < MAX_N; ++i)
        if (!sieve[i]) primes[idx++] = i;
    primes_count = idx;
}

/* --------------------------------------------------------------------- */
/*  Sequence cache                                                        */
/* --------------------------------------------------------------------- */

static seq_t  a_cache[MAX_N];    /* computed a(n) values (1‑based) */
static size_t a_last = 0;        /* highest n that has been computed */

/* --------------------------------------------------------------------- */
/*  Sequence function                                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Guard against n < 1 – behaviour is undefined in this situation. */
    if (n < 1) return (seq_t)0;

    /* Ensure global data is initialised. */
    init_primes();

    /* Extend cache if necessary. */
    while (a_last < (size_t)n) {
        ++a_last;                /* now computing a(a_last) */

        if (a_last == 1) {
            a_cache[0] = (seq_t)2;                 /* a(1) */
            continue;
        }

        /* Recurrence step for a(a_last). */
        seq_t prev = a_cache[a_last - 2];          /* a(a_last-1) */

        /* Retrieve consecutive primes. */
        uint64_t pk      = primes[a_last - 1];     /* p(a_last) */
        uint64_t pk_minus = primes[a_last - 2];    /* p(a_last-1) */

        seq_t target = prev + (seq_t)(pk - pk_minus);
        seq_t cand   = target + (seq_t)1;          /* first number > target */

        while (!is_prime_big(cand)) cand++;        /* seek next prime */

        a_cache[a_last - 1] = cand;                /* a(a_last) */
    }

    return a_cache[n - 1];
}
