/*
 * A080655  a(1) = a(2) = 1; for n >= 2, a(n+1) = a(n) + a(j(n)) where j(m) =
 *          largest divisor of m that is < m.
 * https://oeis.org/A080655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=a(2)=1; for n≥3, a(n)=a(n‑1)+a(j(n‑1)) * where j(m) is the
 *            largest proper divisor of m (m/p where p is the * smallest prime
 *            factor of m). The function memoises previously * computed values
 *            and builds a sieve of smallest prime factors on demand. * It
 *            works for any seq_t/idx_t types supplied b...
 * Range:     u8 n <= 33, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------------------------------------------------------------- */
/*  Static data                                                    */
/* --------------------------------------------------------------- */
static seq_t *cache = NULL;          /* cache[1..]          */
static idx_t  cache_len = 0;         /* allocated size      */
static idx_t  cache_max = 0;         /* largest index cached */

static uint32_t *spf = NULL;         /* smallest prime factor table   */
static idx_t  spf_limit = 0;         /* largest index already sieved  */

/* --------------------------------------------------------------- */
/*  Small‑prime‑factor sieve up to ‘limit’ (inclusive)             */
/* --------------------------------------------------------------- */
static void compute_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;                                 /* already done */

    /* Grow the SPF array if needed. */
    uint32_t *tmp = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!tmp)
        return;                                 /* allocation failed */

    spf = tmp;

    /* Initialise new entries with the number itself. */
    for (idx_t i = spf_limit + 1; i <= limit; ++i)
        spf[i] = (uint32_t)i;

    /* Sieve: for each prime ≤ √limit, mark its multiples. */
    for (idx_t i = 2; i <= limit / i; ++i) {   /* i*i ≤ limit */
        if (spf[i] == (uint32_t)i) {           /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    }

    spf_limit = limit;
}

/* --------------------------------------------------------------- */
/*  Ensure that cache[1..n] is valid                              */
/* --------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;                                 /* already computed */

    /* For the recurrence we need spf values up to n‑1. */
    if (n >= 2)
        compute_spf(n - 1);

    /* Grow the cache array if necessary. */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            return;                     /* allocation failed, leave cache unchanged */
        cache = tmp;
        cache_len = n + 1;
    }

    /* Initialise the two base cases, if they are missing. */
    if (n >= 1 && cache_max < 1)
        cache[1] = (seq_t)1;
    if (n >= 2 && cache_max < 2)
        cache[2] = (seq_t)1;

    /* Update cache_max to at least 2 if we just set the bases. */
    if (cache_max < 2 && n >= 2)
        cache_max = 2;

    /* Compute remaining terms in order. */
    idx_t start = (cache_max >= 2) ? cache_max + 1 : 3;
    for (idx_t i = start; i <= n; ++i) {
        idx_t m = i - 1;                     /* previous index   */
        /* j(m) = largest proper divisor = m / spf[m] */
        uint32_t p = spf[m];
        /* p is always ≥ 2 for m ≥ 2; for m=1 this block is never reached. */
        idx_t j = m / p;
        cache[i] = cache[i - 1] + cache[j];
    }

    cache_max = n;
}

/* --------------------------------------------------------------- */
/*  Public function for A080655                                   */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* undefined for n < 1 */

    ensure_cache(n);
    return cache[n];
}
