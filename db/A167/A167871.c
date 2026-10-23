/*
 * A167871  a(n) = 64^n * Sum_{k=0..n} binomial(2*k,k)^3 / 64^k.
 * https://oeis.org/A167871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167871 – a(n) = 64^n * Sum_{k=0..n} binomial(2k,k)^3 / 64^k
   Recurrence used: a(0) = 1,
                     a(n) = 64 * a(n-1) + binomial(2n,n)^3   for n >= 1            */
#include <stdlib.h>
#include <stdint.h>

/* Static caches for a(n) and binomial(2n,n) values */
static seq_t *cache_a = NULL;  /* holds a(0..k) */
static idx_t  cache_size_a = 0;
static seq_t *cache_b = NULL;  /* holds binomial(2n,n) for n=0..k */
static idx_t  cache_size_b = 0;

/* Ensure a cache can hold indices up to n (inclusive) */
static void ensure_cache(seq_t **arr, idx_t *size, idx_t n)
{
    if (*size > n) return;            /* already sufficient */

    size_t new_size = *size ? *size : 1;
    while (new_size <= n) new_size <<= 1;   /* grow in powers of two */

    seq_t *tmp = (seq_t *)realloc(*arr, new_size * sizeof(seq_t));
    if (!tmp) abort();                /* memory exhaustion */

    if (new_size > *size) {           /* zero‑initialize new slots */
        for (size_t i = *size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
    }

    *arr = tmp;
    *size = new_size;
}

seq_t a(idx_t n)
{
    /* Initialise caches on first call */
    if (!cache_a) {
        cache_a = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache_a) abort();
        cache_size_a = 1;
    }
    if (!cache_b) {
        cache_b = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache_b) abort();
        cache_size_b = 1;
    }

    /* Ensure caches can hold indices up to n */
    ensure_cache(&cache_a, &cache_size_a, n);
    ensure_cache(&cache_b, &cache_size_b, n);

    /* Base value */
    if (cache_a[0] == (seq_t)0) cache_a[0] = (seq_t)1;
    if (cache_b[0] == (seq_t)0) cache_b[0] = (seq_t)1;

    /* Compute from 1 up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache_b[i] == (seq_t)0) {    /* compute binomial(2i,i) */
            seq_t val = cache_b[i - 1];
            /* val = val * 2 * (2*i - 1) / i  (exact division) */
            val = val * ((seq_t)2 * (seq_t)(2 * i - 1));
            val = val / (seq_t)i;
            cache_b[i] = val;
        }

        if (cache_a[i] == (seq_t)0) {    /* a(i) = 64*a(i-1) + binom(2i,i)^3 */
            seq_t cub = cache_b[i];
            cub = cub * cub;        /* cub = binom^2 */
            cub = cub * cache_b[i]; /* cub = binom^3 */

            seq_t prev = cache_a[i - 1];
            prev = prev * ((seq_t)64);   /* 64 * a(i-1) */

            cache_a[i] = prev + cub;
        }
    }

    return cache_a[n];
}
