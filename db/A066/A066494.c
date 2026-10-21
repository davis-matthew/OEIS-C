/*
 * A066494  Numbers n such that prime(n+1) - prime(n) = phi(n).
 * https://oeis.org/A066494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, fini
 * Formula:   a(n) = number n such that prime(n+1) - prime(n) = φ(n)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Prime sieve limit – large enough to cover n up to ~10000
   (prime(10001) ≈ 104743). */
static const idx_t PRIME_LIMIT = 200000ULL;

/* Prime sieve data */
static uint8_t *is_comp = NULL;   /* 0 = prime, 1 = composite */
static uint32_t *pr_list = NULL;  /* primes in ascending order */
static size_t     pr_cnt   = 0;    /* number of primes */

/* Cache for sequence terms */
static seq_t  *cache      = NULL;
static size_t  cache_sz   = 0;    /* number of cached terms */
static size_t  cache_cap  = 0;    /* allocated capacity */

/* Next integer n to test for membership */
static idx_t   max_tested  = 1;   /* start with n = 1 */

/* ------------------------------------------------------------------ */
/* Utility: extend cache array */
static void append_cache(seq_t val)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? 2 * cache_cap : 8;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();  /* fatal: out of memory */
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* ------------------------------------------------------------------ */
/* Utility: compute φ(n) for 64‑bit integer n (n ≤ 10⁴ in practice) */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;          /* result *= (1 - 1/p) */
        }
    }
    if (m > 1)
        result -= result / m;
    return result;
}

/* ------------------------------------------------------------------ */
/* Utility: build prime sieve once */
static void ensure_primes(void)
{
    static bool init = false;
    if (init) return;
    init = true;

    /* Allocate sieve array */
    size_t sz = PRIME_LIMIT + 1;
    is_comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_comp) abort();

    /* Rough upper bound on number of primes up to PRIME_LIMIT
       (π(n) ≈ n / ln n).  20000 is safe for 200000. */
    size_t maxpr = 20000;
    pr_list = (uint32_t *)malloc(maxpr * sizeof(uint32_t));
    if (!pr_list) abort();

    /* Simple sieve of Eratosthenes */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            /* new prime found */
            if (pr_cnt >= maxpr) {
                /* Resize if our rough estimate was too small */
                maxpr *= 2;
                uint32_t *tmp = (uint32_t *)realloc(pr_list, maxpr * sizeof(uint32_t));
                if (!tmp) abort();
                pr_list = tmp;
            }
            pr_list[pr_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* a(0) undefined */

    ensure_primes();                     /* prime data ready */

    /* Grow cache until it contains at least n terms */
    while (cache_sz < (size_t)n && max_tested < pr_cnt) {
        /* Compute φ(max_tested) */
        uint64_t phi_val = phi_uint64((uint64_t)max_tested);

        /* Compute prime gap for this n */
        uint64_t gap = (uint64_t)pr_list[max_tested]
                     - (uint64_t)pr_list[max_tested - 1];

        if (gap == phi_val)
            append_cache((seq_t)max_tested);

        ++max_tested;
    }

    /* Return the requested term if available, otherwise 0 */
    return (n <= cache_sz) ? cache[n - 1] : 0;
}
