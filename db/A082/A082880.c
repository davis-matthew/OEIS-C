/*
 * A082880  Largest value of A075860(j) when j runs through composite numbers
 *          between n-th and (n+1)-th primes. That is, the largest
 *          fixed-point[=prime] reached by iteration of A008472(=sum of prime
 *          factors) initiated with composite values between two consecutive
 *          primes.
 * https://oeis.org/A082880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A082880: Largest fixed point of the iteration of A008472 (sum of distinct prime factors) starting from composite numbers between consecutive primes.
 * Formula (computation by search): for each n ≥ 1, let p = prime(n) and q = prime(n+1).
 * Scan all composites j in (p, q); for each let f(j) be the prime reached by repeatedly replacing x by the sum of distinct prime factors of x until a prime is obtained.
 * a(n) = max{ f(j) | composite j, p < j < q } (0 if no such j).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/* Static global data                                                 */
/* ------------------------------------------------------------------- */
static idx_t   *primes      = NULL;   /* list of primes (0‑based)   */
static size_t   prime_count = 0;      /* number of primes stored    */
static idx_t    prime_limit = 0;      /* last sieve limit          */
static uint32_t *spf        = NULL;   /* smallest prime factor table */

/* Cached results of a(n) */
static seq_t *cache      = NULL;
static bool  *computed   = NULL;
static size_t cache_size = 0;

/* ------------------------------------------------------------------- */
/* Helper: extend primes and spf up to enough count                     */
/* ------------------------------------------------------------------- */
static void ensure_primes_and_spf(idx_t needed)
{
    /* we need at least needed+1 primes (1‑based indexing) */
    if (prime_count >= (size_t)(needed + 1))
        return;

    idx_t limit = prime_limit ? prime_limit * 2 + 100 : 1000;
    while (prime_count < (size_t)(needed + 1)) {
        /* allocate SPF table */
        uint32_t *new_spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
        if (!new_spf) abort();

        /* initialize SPF to i */
        for (idx_t i = 0; i <= limit; ++i)
            new_spf[i] = (uint32_t)i;

        /* sieve */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (new_spf[i] == (uint32_t)i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == (uint32_t)j)
                        new_spf[j] = (uint32_t)i;
            }
        }

        /* build prime list */
        size_t npr = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (new_spf[i] == (uint32_t)i)
                ++npr;

        idx_t *new_pr = (idx_t *)malloc(npr * sizeof(idx_t));
        if (!new_pr) abort();

        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (new_spf[i] == (uint32_t)i)
                new_pr[idx++] = i;

        /* replace old tables */
        free(primes);
        free(spf);
        primes = new_pr;
        spf    = new_spf;
        prime_count = npr;
        prime_limit = limit;

        if (prime_count >= (size_t)(needed + 1))
            break;

        /* increase limit for next attempt */
        limit = prime_limit * 2 + 100;
    }
}

/* ------------------------------------------------------------------- */
/* Sum of distinct prime factors (used in iteration)                   */
/* ------------------------------------------------------------------- */
static inline seq_t sum_distinct_pf(idx_t v)
{
    seq_t sum = 0;
    uint32_t last_p = 0;
    while (v > 1) {
        uint32_t p = spf[(size_t)v];
        if (p != last_p) {
            sum += (seq_t)p;
            last_p = p;
        }
        while (v % p == 0)
            v /= p;
    }
    return sum;
}

/* ------------------------------------------------------------------- */
/* Main sequence function                                             */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* below offset */

    /* Need primes up to (n+1)-th prime */
    ensure_primes_and_spf(n + 1);

    /* Grow cache if needed */
    if (cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        bool *new_comp = (bool *)realloc(computed, new_size * sizeof(bool));
        if (!new_comp) abort();
        /* initialize new entries to false */
        for (size_t i = cache_size; i < new_size; ++i)
            new_comp[i] = false;
        computed = new_comp;
        cache_size = new_size;
    }
    if (computed[n])
        return cache[n];

    /* interval between prime(n) and prime(n+1) */
    idx_t low  = primes[n - 1] + 1;   /* prime(n) + 1   */
    idx_t high = primes[n] - 1;       /* prime(n+1) - 1 */

    seq_t maxPrime = 0;

    for (idx_t j = low; j <= high; ++j) {
        if (spf[(size_t)j] == (uint32_t)j)        /* skip primes */
            continue;

        idx_t x = j;
        /* iterate until x is prime */
        while (spf[(size_t)x] != (uint32_t)x) {
            x = (idx_t)sum_distinct_pf(x);
        }
        seq_t p = (seq_t)x;
        if (p > maxPrime)
            maxPrime = p;
    }

    cache[n]      = maxPrime;
    computed[n]   = true;
    return maxPrime;
}
