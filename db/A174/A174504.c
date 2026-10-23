/*
 * A174504  Continued fraction expansion for exp( Sum_{n>=1} 1/(n*A002203(n))
 *          ), where A002203(n) = (1+sqrt(2))^n + (1-sqrt(2))^n.
 * https://oeis.org/A174504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(n) = 7*a(n-3) - 7*a(n-6) + a(n-9) with a(0)=1, a(1)=1, a(2)=5,
 *            a(3)=1, a(4)=13, a(5)=33, a(6)=1, a(7)=81, a(8)=197
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for computed values */
static seq_t *cache = NULL;        /* array of a(i) values */
static size_t cache_cap = 0;      /* allocated size */
static size_t cache_len = 0;      /* highest index computed + 1 */

/* initial 9 values */
static const seq_t init_vals[9] = {
    1, 1, 5, 1, 13, 33, 1, 81, 197
};

/* ensure the cache can hold indices up to 'needed' (inclusive) */
static void grow_cache(idx_t needed)
{
    size_t new_cap = needed + 1;        /* indices 0..needed */
    if (new_cap <= cache_cap) return;
    /* increase capacity (double strategy) */
    if (cache_cap == 0) cache_cap = 16;
    while (cache_cap < new_cap) cache_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    cache = new_ptr;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 9) return init_vals[n];

    /* grow cache if necessary */
    grow_cache(n);
    if (cache_len == 0) {                 /* first time: initialise cache */
        for (int i = 0; i < 9; ++i) cache[i] = init_vals[i];
        cache_len = 9;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 7*a(i-3) - 7*a(i-6) + a(i-9) */
        seq_t term = cache[i-3];
        term = term * 7;
        term -= cache[i-6] * 7;
        term += cache[i-9];
        cache[i] = term;
    }
    cache_len = n + 1;
    return cache[n];
}
