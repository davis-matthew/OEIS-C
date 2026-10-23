/*
 * A046317  Odd numbers divisible by exactly 4 primes (counted with
 *          multiplicity).
 * https://oeis.org/A046317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 3738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046317: odd integers whose total number of prime factors
   (counted with multiplicity) is exactly 4. The function returns the nth
   such integer, with n >= 1 as 1‑based OEIS indexing. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*  Global storage for the generated sequence and supporting data        */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;     /* cached sequence values                */
static size_t cache_size = 0;        /* number of cached terms                */
static size_t cache_cap  = 0;        /* capacity of the cache                 */

static uint32_t *spf       = NULL;   /* smallest prime factor array           */
static size_t   spf_size   = 0;      /* allocated size                        */
static uint32_t *prime_list = NULL;  /* list of primes up to spf_size         */
static size_t   primes_cap  = 0;     /* capacity of prime list                */
static size_t   primes_len  = 0;     /* number of primes stored               */

static size_t last_processed = 1;    /* last integer examined                 */

/* --------------------------------------------------------------------- */
/*  Build a classic linear sieve up to 'limit' (inclusive).  The array   */
/*  'spf' will be of size limit+1 and filled with the smallest prime    */
/*  factor of each integer.  The array 'prime_list' contains all primes  */
/*  up to 'limit'.  If 'limit' is <= spf_size, nothing is done.          */
/* --------------------------------------------------------------------- */
static void build_spf(size_t limit)
{
    if (limit <= spf_size)
        return;

    /* Reallocate required memory; do not keep the old data */
    free(spf);
    free(prime_list);

    spf        = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    prime_list = (uint32_t *)malloc((limit / 10 + 16) * sizeof(uint32_t));
    if (!spf || !prime_list)
        abort();

    memset(spf, 0, (limit + 1) * sizeof(uint32_t));

    primes_cap = limit / 10 + 16;
    primes_len = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp =
                    (uint32_t *)realloc(prime_list, primes_cap * sizeof(uint32_t));
                if (!tmp)
                    abort();
                prime_list = tmp;
            }
            prime_list[primes_len++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_len &&
                          prime_list[j] <= spf[i] &&
                          i * prime_list[j] <= limit; ++j) {
            spf[i * prime_list[j]] = prime_list[j];
        }
    }
    spf_size = limit + 1;
}

/* --------------------------------------------------------------------- */
/*  Extend the cache so that all odd integers up to 'limit' have been   */
/*  inspected for Ω(n) == 4.  New qualifying terms are appended to 'cache'. */
/* --------------------------------------------------------------------- */
static void extend_cache(size_t limit)
{
    if (limit <= last_processed)
        return;

    build_spf(limit);

    for (size_t x = last_processed + 1; x <= limit; ++x) {
        if ((x & 1) == 0 || x < 3)         /* skip even numbers and 1,2 */
            continue;

        size_t t = x;
        int cnt  = 0;
        while (t > 1 && cnt <= 4) {
            uint32_t p = spf[t];
            t /= p;
            ++cnt;
        }

        if (cnt == 4) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 1024;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)x;
        }
    }
    last_processed = limit;
}

/* --------------------------------------------------------------------- */
/*  Public function required by the tester.                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n==0, return 0 safely */

    /* Ensure the cache contains at least n terms.  Begin with a minimal
       bound that covers the first known term (81). */
    size_t bound = last_processed;
    if (bound < 81)
        bound = 81;

    while (cache_size < (size_t)n) {
        bound *= 2;                    /* double the search bound each iteration */
        extend_cache(bound);
    }

    return cache[n - 1U];
}
