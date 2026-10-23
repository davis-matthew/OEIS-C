/*
 * A195089  Numbers k such that (number of prime factors of k counted with
 *          multiplicity) less (number of distinct prime factors of k) = 5.
 * https://oeis.org/A195089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 777, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195089: numbers k with Ω(k) - ω(k) = 5 (Ω = total prime factors with multiplicity,
   ω = number of distinct prime factors).  The values are generated in
   increasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/*  Prime sieve for smallest prime factors up to LIMIT        */
/*------------------------------------------------------------*/
#define LIMIT 5000000UL  /* enough for the first 10 000 terms */
static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t spf_cap = 0;

/* Initialize the smallest prime factor table */
static void init_spf(void)
{
    if (spf) return;                     /* already initialized */
    spf_cap = LIMIT + 1;
    spf = (uint32_t *)calloc(spf_cap, sizeof(uint32_t));
    if (!spf) abort();
    for (size_t i = 2; i < spf_cap; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i < spf_cap) {
                for (size_t j = i * i; j < spf_cap; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/*------------------------------------------------------------*/
/*  Cache for the generated terms                            */
/*------------------------------------------------------------*/
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_used = 0;    /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t last_tested = 1;    /* last integer that was checked */

/* Ensure the cache can hold at least 'need' terms */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = need > 2 * cache_cap ? need : 2 * cache_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute and store terms until we have 'need' terms */
static void compute_terms_upto(idx_t need)
{
    if (need <= cache_used) return;
    init_spf();

    grow_cache(need);

    idx_t n = last_tested;
    while (true) {
        if (n >= spf_cap) abort();      /* should not happen for the required range */

        /* Factor n using spf */
        uint32_t t = (uint32_t)n;
        int omega = 0;    /* total number of prime factors (with multiplicity) */
        int distinct = 0; /* number of distinct prime factors */

        while (t > 1) {
            uint32_t p = spf[t];
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
            omega += cnt;
            ++distinct;
        }

        if (omega - distinct == 5) {
            cache[cache_used++] = (seq_t)n;
            if (cache_used >= need) break;
        }
        ++n;
    }
    last_tested = n + 1;  /* next integer to test */
}

/*------------------------------------------------------------*/
/*  Public sequence function                                 */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    compute_terms_upto(n);
    return cache[n - 1];
}
