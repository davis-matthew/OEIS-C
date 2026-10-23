/*
 * A032098  "BHK" (reversible, identity, unlabeled) transform of 3,3,3,3,...
 * https://oeis.org/A032098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * (2^(2*n-3) - 2^(n-2) + 1) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A032098 (BHK transform of 3,3,3,3,...) */
/* Recurrence used for n > 3: a(n) = 7*a(n-1) - 14*a(n-2) + 8*a(n-3) */
/* Initial values: a(1)=3, a(2)=6, a(3)=21 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;        /* cache array, 0‑based, cache[i]=a(i+1) */
static idx_t cache_cap = 0;        /* currently allocated capacity */
static idx_t cache_num = 0;        /* number of terms actually computed */

/* ensure that cache can hold at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = (idx_t)new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset starts at 1 – undefined for n=0 */
        return 0;

    /* initialise cache for the first three terms once, if not already */
    if (cache_num == 0) {
        /* make sure we have space for at least three values */
        ensure_capacity(3);
        cache[0] = (seq_t)3;        /* a(1) */
        cache[1] = (seq_t)6;        /* a(2) */
        cache[2] = (seq_t)21;       /* a(3) */
        cache_num = 3;
    }

    /* if n is already computed, return it immediately */
    if (n <= cache_num)
        return cache[n-1];

    /* for n > cache_num, compute terms up to n */
    /* ensure we have enough capacity */
    ensure_capacity(n);

    /* compute recurrence for i = cache_num ... n-1 (0‑based) */
    for (idx_t i = cache_num; i < n; ++i) {
        /* i corresponds to term a(i+1), i ≥ 3 here */
        seq_t val = 7 * cache[i-1] - 14 * cache[i-2] + 8 * cache[i-3];
        cache[i]  = val;
    }
    cache_num = n;
    return cache[n-1];
}
