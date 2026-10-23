/*
 * A146215  a(1) = 1, a(n+1) = a(n) + mod(a(n), p), where p is the lowest
 *          prime such that mod(a(n), p) is nonzero.
 * https://oeis.org/A146215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1
 * a(n+1) = a(n) + (a(n) mod p)  where p is the smallest prime
 *            such that a(n) mod p != 0.  The implementation uses
 *            a cache and a pre‑computed prime list up to a fixed
 *            limit, sufficient for n up to about 10000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Prime table – generated once up to a fixed limit.                  */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 600000u

static bool primes_initialized = false;
static uint64_t *primes = NULL;
static size_t primes_count = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* Simple sieve of Eratosthenes up to PRIME_LIMIT.  The cast is
     * required so that the code compiles as C++17. */
    bool *sieve = (bool*)calloc((size_t)PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve)
        exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;

    for (idx_t p = 2; (uint64_t)p * p <= PRIME_LIMIT; ++p) {
        if (sieve[p]) {
            for (idx_t j = p * p; j <= PRIME_LIMIT; j += p)
                sieve[j] = false;
        }
    }

    /* Count primes. */
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            ++primes_count;

    /* Allocate and store primes.  The cast is required for C++. */
    primes = (uint64_t*)malloc(primes_count * sizeof(uint64_t));
    if (!primes) {
        free(sieve);
        exit(EXIT_FAILURE);
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            primes[idx++] = (uint64_t)i;
        }
    }

    free(sieve);
    primes_initialized = true;
}

/* ------------------------------------------------------------------ */
/* Cache for computed terms.                                          */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;          /* a(1) stored at cache[0] */
static size_t cache_cap = 0;
static idx_t cache_len = 0;          /* number of terms currently stored */

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;  /* safety – a(0) is unused */

    if (cache == NULL) {      /* first call: initialise cache */
        cache_cap = 64;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;     /* a(1) = 1 */
        cache_len = 1;
        init_primes();           /* pre‑compute primes */
    }

    while (cache_len < n) {
        seq_t cur = cache[cache_len - 1];
        seq_t remainder = 0;

        /* Find smallest prime p with cur % p != 0. */
        for (size_t i = 0; i < primes_count; ++i) {
            seq_t prime = (seq_t)primes[i];
            seq_t r = cur % prime;
            if (r != (seq_t)0) {
                remainder = r;
                break;
            }
        }

        /* Compute next term. */
        seq_t next = cur + remainder;

        /* Grow cache if needed.  The cast is required for C++. */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache)
                exit(EXIT_FAILURE);
        }
        cache[cache_len] = next;
        ++cache_len;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n = 0; defensive fallback */
    ensure_cache(n);
    return cache[n - 1];   /* cache[0] holds a(1) */
}
