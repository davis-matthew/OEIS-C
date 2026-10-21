/*
 * A224599  Number of (n+2) X 3 0..2 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) - a(n-5) + a(n-6)
 *            for n > 7, with initial values:
 * a(1) = 122
 * a(2) = 178
 * a(3) = 297
 * a(4) = 416
 * a(5) = 628
 * a(6) = 1045
 * a(7) = 1694
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>       /* for abort in case of allocation failure */

static seq_t *cache = NULL;       /* cache[i] holds a(i+1)   */
static size_t cache_len = 0;      /* number of terms stored  */
static size_t cache_cap = 0;      /* allocated capacity      */

/* initial seven values of the sequence */
static const seq_t init_vals[7] = {
    (seq_t)122,  /* a(1) */
    (seq_t)178,  /* a(2) */
    (seq_t)297,  /* a(3) */
    (seq_t)416,  /* a(4) */
    (seq_t)628,  /* a(5) */
    (seq_t)1045, /* a(6) */
    (seq_t)1694  /* a(7) */
};

/* make sure cache exists and holds the first seven terms */
static void init_cache(void)
{
    if (cache != NULL)          /* already initialized */
        return;

    /* allocate capacity for at least 7 terms */
    cache_cap = 7;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();

    for (size_t i = 0; i < 7; ++i)
        cache[i] = init_vals[i];
    cache_len = 7;
}

/* enlarge cache when needed */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* compute all terms up to index n (1‑based), inclusive */
static void compute_up_to(idx_t n)
{
    /* nothing to do for very small n */
    if (cache_len >= (size_t)n)
        return;

    size_t need = (size_t)n;
    ensure_capacity(need);

    /* recurrence works for zero‑based indices >=6 (i.e., a(7) and beyond) */
    for (size_t i = cache_len; i < need; ++i) {
        /* i corresponds to a(i+1) */
        seq_t val = (seq_t)3 * cache[i - 1];
        val -= (seq_t)3 * cache[i - 2];
        val += (seq_t)2 * cache[i - 3];
        val -= cache[i - 4];
        val -= cache[i - 5];
        val += cache[i - 6];
        cache[i] = val;
    }
    cache_len = need;
}

/* public entry point – returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n < 1, but harmless return */

    init_cache();
    compute_up_to(n);
    return cache[(size_t)n - 1];
}
