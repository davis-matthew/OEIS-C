/*
 * A061764  Positive integers k such that k! is divisible by (k+1)^12.
 * https://oeis.org/A061764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A061764: Positive integers k such that k! is divisible by (k+1)^12.
 * The value is computed by testing successive integers k and collecting the
 * first n such that the divisibility condition holds.  The tests are
 * performed using a precomputed smallest‑prime‑factor (spf) table up to a
 * sufficiently large limit.
 */

#include <stdlib.h>
#include <stdint.h>

/*  ----------  global data and initialization  ----------  */

#define SPF_LIMIT 2000000U          /* a safe upper bound for k+1   */
static uint32_t *spf = NULL;       /* smallest prime factor table */
static int spf_initialized = 0;    /* one‑time initialization flag */

/* light‑weight dynamic array for cached sequence values */
static seq_t *cache_vals = NULL;
static size_t cache_len = 0; /* number of cached terms */
static size_t cache_cap = 0; /* allocated capacity */

/* keep track of the next k to test (last checked k) */
static uint64_t next_k = 1;  /* start after 1, so first tested k is 2 */

/* allocate and compute the spf table */
static void init_spf(void)
{
    if (spf_initialized) return;
    spf = (uint32_t *)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
    for (uint32_t i = 2; i <= SPF_LIMIT; ++i)
    {
        if (!spf[i])            /* i is prime */
        {
            spf[i] = i;
            if ((uint64_t)i * i <= SPF_LIMIT)
            {
                for (uint32_t j = i * i; j <= SPF_LIMIT; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }
    spf_initialized = 1;
}

/* append a value to the cache, expanding as needed */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
        if (!new_arr) abort(); /* memory exhausted */
        cache_vals = new_arr;
        cache_cap = new_cap;
    }
    cache_vals[cache_len++] = val;
}

/* check whether k! is divisible by (k+1)^12 */
static int check_k(uint64_t k)
{
    uint64_t m = k + 1;       /* k+1 */
    while (m > 1)
    {
        uint32_t p = spf[m];
        int e = 0;
        while ((m % p) == 0)
        {
            ++e;
            m /= p;
        }

        /* compute exponent of p in k! via Legendre's formula */
        uint64_t sum = 0;
        uint64_t t = k;
        while (t)
        {
            t /= p;
            sum += t;
        }

        if (sum < (uint64_t)12 * (uint64_t)e)
            return 0;  /* fails for this prime */
    }
    return 1;  /* all prime factors satisfied */
}

/*  ----------  exported sequence function  ----------  */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* never used, but guards against 0 */
    init_spf();

    /* generate terms until we have at least n terms cached */
    while (cache_len < (size_t)n)
    {
        ++next_k;  /* test next integer k */
        if (next_k > (uint64_t)SPF_LIMIT) abort(); /* safety check */
        if (check_k(next_k))
            cache_push((seq_t)next_k);
    }
    return cache_vals[n - 1];
}
