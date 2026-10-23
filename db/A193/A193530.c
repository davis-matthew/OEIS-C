/*
 * A193530  Expansion of (1 - 2*x - 2*x^2 + 3*x^3 +
 *          x^5)/((1-x)*(1-2*x-x^2)*(1-2*x^2-x^4)).
 * https://oeis.org/A193530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + 3*a(n-4) - a(n-5) + a(n-6)
 *            + a(n-7) Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=7,
 *            a(5)=13, a(6)=31
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* array that stores a(0)...a(k) */
static idx_t  cache_len  = 0;       /* number of terms currently stored   */
static size_t cache_cap  = 0;       /* current capacity of the array      */

/* enlarge cache so that it can hold at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= (size_t)needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)needed)
        new_cap = needed;
    /* realloc may be called with NULL pointer */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise cache with the first seven terms */
static void initialise_cache(void)
{
    if (cache_len != 0)   /* already initialised */
        return;
    ensure_capacity(7);
    cache[0] = (seq_t)1;   /* a(0) */
    cache[1] = (seq_t)1;   /* a(1) */
    cache[2] = (seq_t)2;   /* a(2) */
    cache[3] = (seq_t)3;   /* a(3) */
    cache[4] = (seq_t)7;   /* a(4) */
    cache[5] = (seq_t)13;  /* a(5) */
    cache[6] = (seq_t)31;  /* a(6) */
    cache_len = 7;
}

seq_t a(idx_t n)
{
    initialise_cache();

    if (n < cache_len)
        return cache[n];

    ensure_capacity(n + 1);

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 3 * cache[i-1]                      /* 3*a(i-1) */
                  + cache[i-2]                          /* + a(i-2)  */
                  - 7 * cache[i-3]                      /* -7*a(i-3) */
                  + 3 * cache[i-4]                      /* +3*a(i-4) */
                  - cache[i-5]                          /* - a(i-5) */
                  + cache[i-6]                          /* + a(i-6) */
                  + cache[i-7];                         /* + a(i-7) */
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
