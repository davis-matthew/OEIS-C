/*
 * A023956  Expansion of 1/((1-x)(1-6x)(1-8x)(1-11x)).
 * https://oeis.org/A023956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*11^(n+3) - 25*8^(n+3) + 21*6^(n+3) - 3)/1050
 *            Implemented via linear recurrence: a(0)=1, a(1)=26, a(2)=449,
 *            a(3)=6502 a(n) = 26*a(n-1) - 227*a(n-2) + 730*a(n-3) -
 *            528*a(n-4) (n>=4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached terms */
static idx_t cache_size = 0;    /* terms computed so far */
static idx_t cache_cap = 0;     /* allocated capacity */

/* Expand cache to hold at least cap terms */
static void ensure_capacity(idx_t cap)
{
    if (cap <= cache_cap)
        return;
    /* grow capacity at least double */
    if (cache_cap == 0)
        cache_cap = 4;
    while (cache_cap < cap)
        cache_cap *= 2;
    /* reallocate memory */
    seq_t *new_cache = (cache == NULL) ? 
        (seq_t*)malloc(sizeof(seq_t) * cache_cap) :
        (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
    cache = new_cache;
    /* initialize new slots to zero (not strictly needed) */
    for (idx_t i = cache_size; i < cache_cap; ++i)
        cache[i] = 0;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialize cache with first four terms */
    if (cache == NULL) {
        ensure_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)449;
        cache[3] = (seq_t)6502;
        cache_size = 4;
    }

    if (n < cache_size)
        return cache[n];

    ensure_capacity(n + 1);

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum1 = cache[i-1] * (seq_t)26 + cache[i-3] * (seq_t)730;
        seq_t sum2 = cache[i-2] * (seq_t)227 + cache[i-4] * (seq_t)528;
        cache[i] = sum1 - sum2;
    }
    cache_size = n + 1;
    return cache[n];
}
