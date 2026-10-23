/*
 * A184163  a(0)=1, a(1)=1, a(n)=a(n-1)^a(n-2)+1.
 * https://oeis.org/A184163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(n)=a(n-1)^a(n-2)+1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;       /* cached values of the sequence */
static size_t cache_cap = 0;     /* current capacity of the cache */
static idx_t  last_computed = 1; /* highest index that has been computed */

/* Make sure the cache can hold at least indices 0..n.
   If the cache is NULL, it is created and seeded with a(0)=1, a(1)=1. */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 4;                /* allocate a few slots first */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;          /* allocation failed – caller will handle */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* expand capacity if needed */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;            /* allocation failed – caller will handle */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of A184163, storing computed values for future calls. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    ensure_capacity(n);

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = big_pow(cache[i-1], cache[i-2]) + (seq_t)1;
    }
    last_computed = n;

    return cache[n];
}
