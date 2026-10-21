/*
 * A085234  (Greatest power of smallest prime factor of n) < square root(n).
 * https://oeis.org/A085234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m > 1 such that (p^e) < sqrt(m), where p =
 *            smallest prime factor of m and p^e is the largest power of p
 *            dividing m
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Static state used by the sequence function and helper routines.   */
/* ------------------------------------------------------------------ */
static seq_t *cache            = NULL;   /* cache[1] is the first term */
static size_t cache_cap        = 0;      /* allocated capacity of cache  */
static size_t cache_len        = 0;      /* number of terms stored         */

static uint64_t *spf            = NULL;   /* smallest prime factor array   */
static size_t spf_cap           = 0;      /* allocated capacity of spf     */

static uint64_t next_candidate  = 2;      /* next natural number to test    */

/* ------------------------------------------------------------------ */
/*  Auxiliary routine: grow the cache array to at least 'needed'     */
/* ------------------------------------------------------------------ */
static void grow_cache(size_t needed)
{
    if (needed < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Auxiliary routine: extend the spf array up to 'limit' (inclusive).*/
/*  The array is filled with the smallest prime factor of each index. */
/* ------------------------------------------------------------------ */
static void extend_spf(size_t limit)
{
    if (limit + 1 <= spf_cap)
        return; /* already long enough */

    size_t new_cap = spf_cap ? spf_cap * 2 : 1024;
    while (new_cap <= limit + 1)
        new_cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(spf, new_cap * sizeof(uint64_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    spf = tmp;
    /* 0 and 1 are unused; initialise new slots to 0. */
    for (size_t i = spf_cap; i < new_cap; ++i)
        spf[i] = 0;

    /* Sieve to fill smallest prime factors for new entries. */
    for (size_t i = 2; i < new_cap; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            if (i <= new_cap / i) {
                for (size_t j = i * i; j < new_cap; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    spf_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Sequence function for OEIS A085234                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /*  Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n];

    /*  Ensure enough space in the cache. */
    if (n >= cache_cap)
        grow_cache(n);

    /*  Generate terms until the desired index is reached. */
    while (cache_len < n) {
        uint64_t m = next_candidate;

        /*  Make sure the spf table covers 'm'. */
        extend_spf(m);

        uint64_t p = spf[m];
        if (p == 0)          /* safety: m might be 0 or 1, but we start from 2 */
            p = m;           /* treat as prime if pf not set */

        /*  Compute the greatest power of p dividing m. */
        uint64_t tmp = m;
        uint64_t pow_p = 1;
        while ((tmp % p) == 0) {
            tmp /= p;
            pow_p *= p;
        }

        /*  Check the defining inequality: pow_p < sqrt(m). */
        if ((__int128)pow_p * pow_p < (__int128)m) {
            /*  Store the new term.  cache[1] is the first term. */
            ++cache_len;
            cache[cache_len] = (seq_t)m;
        }

        ++next_candidate;
    }

    return cache[n];
}
