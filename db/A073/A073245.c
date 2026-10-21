/*
 * A073245  Sum of all cubefree numbers with the same squarefree kernel as the
 *          n-th squarefree number.
 * https://oeis.org/A073245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A062822(n) * A005117(n) where A062822(n) = ∏_{p |
 *            A005117(n)} (1 + p) and A005117(n) is the n‑th squarefree
 *            integer. This code computes a(n) for any n ≥ 1 using a dynamic
 *            sieve that grows as needed. The implementation is O(n log log n)
 *            in total and uses no external libraries
 * Range:     u8 n <= 9, u16 n <= 113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----------  static storage  ---------- */
static uint32_t *spf = NULL;          /* smallest prime factor up to current limit */
static uint8_t  *is_sf = NULL;        /* square‑free flag up to current limit */
static uint64_t *squarefree = NULL;   /* array of square‑free numbers in order */
static size_t     sf_limit = 0;       /* highest index sieved so far */
static size_t     sf_count  = 0;       /* number of square‑free numbers stored */

/* ----------  helper: recompute sieve from scratch up to 'limit'  ---------- */
static void recompute_sieve(size_t limit)
{
    /* allocate and initialise spf */
    uint32_t *new_spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                    /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* allocate and initialise is_sf (true=1) */
    uint8_t *new_is_sf = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    memset(new_is_sf, 1, limit + 1);
    new_is_sf[0] = new_is_sf[1] = 1;              /* 0 and 1 are square‑free */

    /* mark non‑square‑free numbers (i^2 divides n) */
    for (size_t i = 2; i * i <= limit; ++i)
        if (new_spf[i] == (uint32_t)i) {          /* i is prime */
            size_t sq = i * i;
            for (size_t j = sq; j <= limit; j += sq)
                new_is_sf[j] = 0;
        }

    /* collect all square‑free numbers into an array */
    size_t count = 0;
    for (size_t i = 1; i <= limit; ++i)
        if (new_is_sf[i]) ++count;

    uint64_t *new_squarefree = (uint64_t*)malloc(count * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 1; i <= limit; ++i)
        if (new_is_sf[i]) new_squarefree[idx++] = i;

    /* free old data */
    free(spf);
    free(is_sf);
    free(squarefree);

    /* update globals */
    spf = new_spf;
    is_sf = new_is_sf;
    squarefree = new_squarefree;
    sf_limit = limit;
    sf_count = count;
}

/* ----------  ensure we have at least 'n' square‑free numbers  ---------- */
static uint64_t nth_squarefree(idx_t n)
{
    if ((size_t)n <= sf_count) return squarefree[n - 1];

    /* extend limit until we have enough */
    size_t new_limit = sf_limit ? sf_limit * 2 : 1000;
    while (sf_count < (size_t)n) {
        if (new_limit < sf_limit) {               /* overflow guard */
            new_limit = (size_t)-1;
            break;
        }
        recompute_sieve(new_limit);
        if (sf_count >= (size_t)n) break;
        new_limit *= 2;
    }

    return squarefree[n - 1];
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    uint64_t k = nth_squarefree(n);          /* A005117(n) */
    seq_t prod = 1;

    /* factor k using the precomputed spf array */
    uint64_t t = k;
    while (t > 1) {
        uint32_t p = spf[t];
        if (p == 0) p = (uint32_t)t;         /* safety for unexpected values */
        prod *= (seq_t)(p + 1);
        while (t % p == 0) t /= p;
    }

    return (seq_t)k * prod;                  /* A062822(n) * k */
}
