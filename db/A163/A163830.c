/*
 * A163830  The n-th composite minus the product of the indices of the primes
 *          in its prime factorization.
 * https://oeis.org/A163830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n) - A003963(A002808(n)) (the n‑th composite
 *            minus the product of the indices of the primes in its prime
 *            factorisation)
 * Range:     u8 n <= 212, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, ensure_capacity, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/*  Standard includes                                                      */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Utilities available in the build environment                         */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t n)         */
#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t n,
                                       uint64_t *pr, uint64_t *ex,
                                       size_t *cnt)                */
#include "../utils/build_primes.h"     /* void build_primes(idx_t limit,
                                       idx_t **out, size_t *cnt)   */
#include "../utils/ensure_capacity.h"  /* void ensure_capacity(size_t needed,
                                       size_t *cap, void **ptr,
                                       size_t elem_size)           */

/* --------------------------------------------------------------------- */
/*  Prime data – generated once and reused for all calls                 */
/* --------------------------------------------------------------------- */
static const idx_t PRIME_LIMIT = 200000;   /* enough for n <= 10000        */
static bool primes_inited = false;
static uint32_t *prime_index = NULL;      /* prime_index[p] = 1‑based index of prime p, 0 if not prime */
static idx_t  *prime_list   = NULL;
static size_t  prime_count   = 0;

/* --------------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes_inited)
        return;

    /* build list of primes up to PRIME_LIMIT */
    idx_t *primes = NULL;
    size_t pcnt = 0;
    build_primes(PRIME_LIMIT, &primes, &pcnt);

    /* build index map */
    prime_index = (uint32_t*)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    for (size_t i = 0; i < pcnt; ++i)
        prime_index[primes[i]] = (uint32_t)(i + 1);  /* indices are 1‑based */

    prime_list  = primes;
    prime_count = pcnt;
    primes_inited = true;
}

/* --------------------------------------------------------------------- */
/*  Cache of n‑th composite numbers                                      */
/* --------------------------------------------------------------------- */
static idx_t *comp_cache = NULL;
static size_t comp_cap   = 0;
static size_t comp_len   = 0;

/* --------------------------------------------------------------------- */
static void ensure_composites(size_t needed)
{
    if (comp_len >= needed)
        return;
    if (comp_cap < needed)
        ensure_capacity(needed, &comp_cap, (void**)&comp_cache, sizeof(idx_t));
}

/* --------------------------------------------------------------------- */
static idx_t nth_composite(idx_t n)
{
    if (comp_cache == NULL)
        comp_cap = 0;      /* first call – initialise capacity flag */

    ensure_composites(n);

    /* start searching from the last stored composite so we don't redo work */
    idx_t m = comp_len ? comp_cache[comp_len - 1] + 1 : 4;  /* 4 is the first composite */

    while (comp_len < (size_t)n) {
        /* find next composite */
        while (is_prime(m))
            ++m;
        comp_cache[comp_len++] = m;
        ++m;      /* move to the next candidate for the next iteration */
    }
    return comp_cache[n - 1];
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Prepare prime tables once */
    init_primes();

    /* 1. Get the n-th composite number m */
    idx_t m = nth_composite(n);    /* m >= 4 */

    /* 2. Factorise m */
    uint64_t m64 = (uint64_t)m;
    uint64_t pr[64], ex[64];
    size_t pcnt = 0;
    factor_u64(m64, pr, ex, &pcnt);

    /* 3. Compute product of prime indices (with multiplicity) */
    seq_t product = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        uint32_t idx;

        /* look up the index of p; all primes below PRIME_LIMIT are indexed */
        if (p <= (uint64_t)PRIME_LIMIT)
            idx = prime_index[(size_t)p];
        else {
            /* fallback: compute the index by incremental search – this
               situation will never occur for n <= 10000 (m <= ~200k).        */
            idx = 0;
            for (idx_t candidate = 2; candidate <= (idx_t)p; ++candidate)
                if (is_prime(candidate)) {
                    ++idx;
                    if ((uint64_t)candidate == p)
                        break;
                }
        }

        /* multiply idx e times */
        for (uint64_t k = 0; k < e; ++k)
            product *= (seq_t)idx;
    }

    /* 4. Result is m minus that product */
    return (seq_t)m - product;
}
