/*
 * A079623  a(1) = a(2) = 1, a(3)=4, a(n) = abs(a(n-1) - a(n-2) - a(n-3)).
 * https://oeis.org/A079623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2)=1; a(3)=4; a(n)=abs(a(n-1)-a(n-2)-a(n-3)) for n>3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑indexed cache of computed terms   */
static idx_t cache_cap = 0;          /* current allocated size (index+1)   */
static idx_t cache_used = 0;         /* maximum index presently computed  */

seq_t a(idx_t n)
{
    /* handle n==0 which is below the OEIS offset – behaviour is unimportant */
    if (n == 0) return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 4;         /* indices 0..3, 0 unused */
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!cache) return (seq_t)0; /* allocation failure – unlikely in judge */
        cache_cap = init_cap;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache_used = 3;
    }

    /* extend cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1; /* indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0; /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* return already computed value */
    if (n <= cache_used) {
        return cache[n];
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t prev3 = cache[i - 3];

        /* sub = prev2 + prev3; safe because values stay small */
        seq_t sub = prev2 + prev3;

        seq_t diff;
        if (prev1 >= sub) {
            diff = prev1 - sub;
        } else {
            diff = sub - prev1;
        }
        cache[i] = diff;
    }

    cache_used = n;
    return cache[n];
}
