/*
 * A028190  Expansion of 1/((1-5*x)*(1-8*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A028190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 90*a(n-2) + (8^(n+1) - 5^(n+1))/3; a(0)=1,
 *            a(1)=32, a(2)=647, a(3)=10570
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values of a(n)      */
static idx_t cache_len = 0;      /* number of allocated slots    */
static seq_t p5 = 0;             /* 5^(next_index+1)            */
static seq_t p8 = 0;             /* 8^(next_index+1)            */
static idx_t next_compute = 0;   /* next index that will be computed   */

static void grow_cache(idx_t new_len)
{
    if (new_len <= cache_len) return;
    size_t new_cap = cache_len ? cache_len : 4;
    while (new_cap < new_len) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure          */
    cache = tmp;
    cache_len = new_cap;
}

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;

    /* initialize cache and cache[0..3] if needed */
    if (!cache) {
        grow_cache(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)647;
        cache[3] = (seq_t)10570;

        /* initialise powers for the next index to compute:
           next_compute = 4  --> need 5^(5) and 8^(5) */
        next_compute = 4;
        p5 = (seq_t)5;
        for (idx_t i = 0; i < 4; ++i) p5 *= (seq_t)5;   /* 5^5   */
        p8 = (seq_t)8;
        for (idx_t i = 0; i < 4; ++i) p8 *= (seq_t)8;   /* 8^5   */
    }

    /* if already computed, return cached value */
    if (n < cache_len && n < next_compute) {
        return cache[n];
    }

    /* compute missing values up to n */
    grow_cache((idx_t)n + 1);
    for (idx_t i = next_compute; i <= n; ++i) {
        /* compute term (8^(i+1) - 5^(i+1))/3, with current p8,p5 */
        seq_t term = (p8 - p5) / (seq_t)3;
        seq_t val = cache[i-1] * (seq_t)19;
        val -= cache[i-2] * (seq_t)90;
        val += term;
        cache[i] = val;

        /* update powers for next iteration: multiply by 8 and by 5 respectively */
        p5 *= (seq_t)5;
        p8 *= (seq_t)8;
    }
    next_compute = n + 1;
    return cache[n];
}
