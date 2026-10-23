/*
 * A145536  a(n) is the number of numbers removed in each step of
 *          Eratosthenes's sieve for 9!.
 * https://oeis.org/A145536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) is the number of integers removed at the n‑th step of
 *            Eratosthenes' sieve for 9! (the n‑th prime pₙ crosses out all
 *            composites whose smallest prime factor is pₙ). Maximum n is the
 *            number of primes ≤ 9!=362880
 * Range:     u8 none, u16 none, u32 n <= 110, u64 n <= 110, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

static const idx_t N = 362880ULL;           /* 9! */

static uint32_t *spfs = NULL;               /* smallest prime factor of each i */
static uint16_t *prime_of = NULL;           /* prime → index (1‑based) */
static uint32_t *primes = NULL;             /* list of primes up to N */
static seq_t   *removals = NULL;            /* a(n) values */
static size_t  prime_cnt = 0;

/* initialize once */
static void init_cache(void)
{
    if (spfs != NULL)
        return; /* already initialized */

    /* allocate arrays */
    spfs = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    if (!spfs) abort();
    prime_of = (uint16_t *)calloc((size_t)N + 1, sizeof(uint16_t));
    if (!prime_of) abort();
    primes = (uint32_t *)malloc(40000 * sizeof(uint32_t)); /* rough over‑estimate */
    if (!primes) abort();

    /* sieve for smallest prime factor and prime list */
    for (idx_t i = 2; i <= N; ++i) {
        if (spfs[i] == 0) {
            spfs[i] = (uint32_t)i;                /* i is prime */
            if (prime_cnt >= 40000) { /* grow if necessary */
                size_t new_cap = 40000 * 2;
                primes = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[prime_cnt++] = (uint32_t)i;
            prime_of[(size_t)i] = (uint16_t)(prime_cnt); /* 1‑based */

            if (i > N / i) continue;              /* avoid overflow */
            for (idx_t j = i * i; j <= N; j += i)
                if (spfs[j] == 0)
                    spfs[j] = (uint32_t)i;
        }
    }

    /* allocation for removal counts */
    removals = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    if (!removals) abort();
    for (size_t i = 0; i < prime_cnt; ++i)
        removals[i] = 0;

    /* count composites by their smallest prime factor */
    for (idx_t i = 2; i <= N; ++i) {
        uint32_t p = spfs[i];
        if (i == p) continue;            /* i is prime, not removed */
        size_t idx = (size_t)prime_of[p] - 1; /* 0‑based */
        assert(idx < prime_cnt);
        removals[idx] += 1;
    }
}

/* Public function */
seq_t a(idx_t n)
{
    init_cache();
    if (n == 0 || n > prime_cnt)
        return 0;
    return removals[(size_t)n - 1];
}
