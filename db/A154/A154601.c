/*
 * A154601  Primes of the form 2*n^2 + 22*n + 9.
 * https://oeis.org/A154601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154601:  a(n) is the n‑th prime of the form 2*k² + 22*k + 9.
   The function generates successive values of k, tests 2*k²+22*k+9 for
   primality using a small sieve, and returns the n‑th prime found.   */

/* C11 / C++17 compatible implementation */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Small prime table for trial division                                    */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT     200000     /* > sqrt(max 2*k²+22*k+9 for k ≤ 10⁴) */
#define PRIME_CAPACITY  20000      /* number of primes we reserve */

static uint64_t small_primes[PRIME_CAPACITY];
static size_t   prime_count = 0;

/* Initialise small_primes on first call */
static void init_small_primes(void)
{
    if (prime_count > 0) return;                 /* already initialised */

    bool *mark = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!mark) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!mark[i]) {
            if (prime_count < PRIME_CAPACITY)
                small_primes[prime_count++] = (uint64_t)i;
            else
                break;                          /* safety */

            if (i * i <= SIEVE_LIMIT) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    mark[j] = true;
            }
        }
    }
    free(mark);
}

/* Deterministic primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Cache of the sequence values                                            */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of seq_t values */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  next_k     = 1;      /* next k to test (starting at 1) */

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – undefined for n<1 */

    /* initialise the small‑prime sieve */
    init_small_primes();

    /* extend the cache until we have at least n primes */
    while (cache_len < (size_t)n) {
        /* compute candidate value 2*k² + 22*k + 9 using 64‑bit arithmetic */
        uint64_t k  = (uint64_t)next_k;
        uint64_t val = 2 * k * k + 22 * k + 9;

        /* test primality */
        if (is_prime_uint64(val)) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap ? cache_cap * 2 : 8);
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE); /* allocation failure */
                cache = tmp;
            }
            /* store the prime */
            cache[cache_len++] = (seq_t)val;
        }
        ++next_k;   /* move to next k */
    }

    return cache[n - 1];
}
