/*
 * A063638  Primes p such that p-2 is a semiprime.
 * https://oeis.org/A063638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A063638:
 *  Primes p such that p‑2 is a product of two primes (a semiprime).
 *  The function builds a sieve of primes once and then scans the
 *  primes, testing semiprimality of p‑2 by trial division with the
 *  prime list.  Results are memoised so that non‑monotonic calls to a(n)
 *  are inexpensive.  All arithmetic is performed with native integer
 *  types; the returned value is cast to seq_t which is permitted to be
 *  an arbitrary‑precision integer.  The implementation avoids any
 *  floating‑point sqrt call and therefore works for both the 64‑bit
 *  and arbitrary‑precision build configurations.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*  Parameters and static state                                          */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 20000000   /* sufficient for ≥ 10000 terms within limits */

static uint8_t  *is_prime = NULL;          /* sieve bitmap: 1 = prime, 0 = composite */
static uint32_t *prime_list = NULL;        /* list of primes extracted from the sieve */
static size_t    prime_count = 0;          /* number of primes stored */

static seq_t *cache = NULL;                /* memoised sequence terms */
static size_t cache_count = 0;             /* number of terms cached */
static size_t cache_capacity = 0;

static size_t next_prime_index = 0;        /* index of next prime in prime_list to test */

/* --------------------------------------------------------------------- */
/*  Sieve construction                                                   */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (is_prime != NULL) return;            /* already initialised */

    /* allocate sieve bitmap */
    is_prime = (uint8_t *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) abort();
    memset(is_prime, 1, SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    /* classic Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    /* collect primes into an array */
    prime_count = 0;
    size_t estimate = SIEVE_LIMIT / 10;   /* rough upper bound */
    prime_list = (uint32_t *)malloc(estimate * sizeof(uint32_t));
    if (!prime_list) abort();

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_count == estimate) {
                estimate *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, estimate * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_count++] = i;
        }
    }

    /* initialise memoisation cache */
    cache_capacity = 128;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    if (!cache) abort();
    cache_count = 0;
    next_prime_index = 0;
}

/* --------------------------------------------------------------------- */
/*  Utility: grow memoisation cache                                       */
/* --------------------------------------------------------------------- */
static void cache_grow(void)
{
    if (cache_count < cache_capacity) return;
    cache_capacity *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
}

/* --------------------------------------------------------------------- */
/*  Build memoisation cache up to required index                         */
/* --------------------------------------------------------------------- */
static void build_to(idx_t want)
{
    init_sieve();

    while (cache_count < (size_t)want) {
        if (next_prime_index >= prime_count) abort();  /* sieve too small */

        uint32_t p = prime_list[next_prime_index++];
        uint32_t x = p - 2;                         /* candidate semiprime */

        if (x < 4) continue;                       /* 4 = 2*2 is smallest */

        uint32_t divisor = 0;

        /* trial division using the prime list */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t pr = prime_list[i];
            uint64_t pr_sq = (uint64_t)pr * pr;
            if (pr_sq > x) break;
            if (x % pr == 0) { divisor = pr; break; }
        }

        if (divisor == 0) continue;                /* x is prime → not semiprime */
        uint32_t q = x / divisor;                  /* the second factor */

        if (!is_prime[(size_t)q]) continue;        /* second factor not prime */

        /* both factors are prime → accept p */
        cache_grow();
        cache[cache_count++] = (seq_t)p;
    }
}

/* --------------------------------------------------------------------- */
/*  Public API                                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; graceful fallback */

    build_to(n);
    return cache[(size_t)(n - 1)];  /* sequence is 1‑based */
}
