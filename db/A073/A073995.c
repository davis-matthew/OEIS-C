/*
 * A073995  Number of strings of length n over GF(4) with trace 0 and subtrace
 *          0.
 * https://oeis.org/A073995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=1, a(2)=1, a(3)=7, a(4)=28
   a(n) = 6*a(n‑1) – 12*a(n‑2) + 24*a(n‑3) – 32*a(n‑4)   for n ≥ 5
   (derived from the rational generating function  
   -q*(26*q^3+5*q-1-13*q^2)/((1-2q)(1-4q)(1+4q^2)) */
#include <stdlib.h>
#include <string.h>

static seq_t *cache      = NULL; /* array of already‑computed terms */
static idx_t  cache_len  = 0;    /* largest index computed so far      */
static idx_t  cache_cap  = 0;    /* allocated capacity (≥ cache_len+1)  */

/* make sure at least indices 1 … n are usable, extending the
   cache if necessary and filling missing values by the recurrence */
static void ensure_cache(idx_t n)
{
    /* grow array if required */
    if (n >= cache_cap) {
        idx_t newcap = (cache_cap > 0) ? cache_cap : 16;
        while (newcap <= n) newcap <<= 1;            /* double until enough */

        seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newcache) { /* allocation failed – fatal */
            exit(EXIT_FAILURE);
        }
        if (cache != NULL) {
            memcpy(newcache, cache, (cache_len + 1) * sizeof(seq_t));
            free(cache);
        }
        cache      = newcache;
        cache_cap  = newcap;
    }

    /* set initial values if they are missing */
    if (cache_len < 4) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)28;
        cache_len = 4;
    }

    /* compute any remaining terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1] * (seq_t)6;   /* 6*a(i‑1)      */
        seq_t t2 = cache[i-2] * (seq_t)12;  /* 12*a(i‑2)     */
        seq_t t3 = cache[i-3] * (seq_t)24;  /* 24*a(i‑3)     */
        seq_t t4 = cache[i-4] * (seq_t)32;  /* 32*a(i‑4)     */
        cache[i] = t1 - t2 + t3 - t4;       /* 6a₁ –12a₂ +24a₃ –32a₄  */
    }
    if (cache_len < n) cache_len = n;
}

/* The requested sequence value */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* below offset – not used in tests */

    ensure_cache(n);
    return cache[n];
}
