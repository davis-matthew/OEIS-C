/*
 * A162361  Central prime factor of A014612(n).
 * https://oeis.org/A162361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the middle prime among the three prime factors (with
 *            multiplicity) of the n-th positive integer having exactly 3
 *            prime factors (Ω(m)=3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Global state                                                    */
/* --------------------------------------------------------------------- */
static seq_t *a_cache = NULL;      /* Cached values of a(n)                  */
static size_t cache_sz  = 0;       /* Number of cached terms                 */
static size_t cache_cap = 0;       /* Allocated capacity for a_cache         */

static uint32_t *spf = NULL;       /* SPF[i] = smallest prime factor of i    */
static size_t spf_cap  = 0;        /* Current upper bound for SPF array      */

static uint32_t next_i = 2;        /* Next integer to test for Ω(m)=3       */

/* --------------------------------------------------------------------- */
/*  Helper: Linear sieve to build SPF up to LIMIT (inclusive)          */
/* --------------------------------------------------------------------- */
static void build_spf(size_t LIMIT)
{
    if (LIMIT <= spf_cap)
        return;                     /* already built up to this limit        */

    /* Reallocate spf array */
    uint32_t *new_spf = (uint32_t*)realloc(spf, (LIMIT + 1) * sizeof(uint32_t));
    if (!new_spf)
        return;                     /* memory failure not expected           */
    spf = new_spf;

    /* Initialize spf entries for new range */
    for (size_t i = spf_cap + 1; i <= LIMIT; ++i)
        spf[i] = 0;

    /* Linear sieve */
    size_t *prime_list = (size_t*)calloc(LIMIT / 10 + 1, sizeof(size_t));
    if (!prime_list)
        return;                     /* memory failure not expected           */
    size_t prime_cnt = 0;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (prime_cnt < LIMIT / 10 + 1)
                prime_list[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            size_t p = prime_list[j];
            if (p > spf[i] || i * p > LIMIT)
                break;
            spf[i * p] = (uint32_t)p;
        }
    }
    free(prime_list);
    spf_cap = LIMIT;
}

/* --------------------------------------------------------------------- */
/*  Ensure we have at least N terms cached                              */
/* --------------------------------------------------------------------- */
static void ensure_cache(size_t N)
{
    if (cache_sz >= N)
        return;                     /* already have enough cache              */

    /* Resize cache if necessary */
    if (cache_cap < N) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < N)
            new_cap *= 2;
        seq_t *new_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return;                 /* memory failure not expected           */
        a_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Ensure SPF table covers the numbers we will test */
    if (!spf) {
        build_spf(2000000);          /* safe initial limit for n≤10000       */
    }

    /* Generate terms until we reach N */
    while (cache_sz < N) {
        /* Extend SPF if we are out of range */
        if ((size_t)next_i > spf_cap)
            build_spf((size_t)next_i * 2);

        uint32_t m = next_i;
        uint32_t temp = m;
        int occ = 0;
        seq_t f1 = 0, f2 = 0, f3 = 0;

        while (temp > 1 && occ <= 3) {
            uint32_t p = spf[temp];
            temp /= p;
            if (occ == 0) f1 = p;
            else if (occ == 1) f2 = p;
            else if (occ == 2) f3 = p;
            occ++;
        }

        if (occ == 3)                 /* m has exactly three prime factors   */
            a_cache[cache_sz++] = f2; /* central prime is the second factor  */

        next_i++;                      /* advance to next integer               */
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the build                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* sequence offset is 1, so undefined  */
    size_t idx = (size_t)n;
    ensure_cache(idx);
    return a_cache[n - 1];            /* cached value for n-th term             */
}
