/*
 * A100296  Expansion of g.f. x*(1+2*x-x^2)/(1-4*x-2*x^2+x^3).
 * https://oeis.org/A100296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = 4*a(n-1) + 2*a(n-2) - a(n-3)
   Initial values: a(1)=1, a(2)=6, a(3)=25
   OEIS: A100296 (Expansion of g.f. x*(1+2*x-x^2)/(1-4*x-2*x^2+x^3)) */
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>   /* for uint64_t etc. */

static seq_t *cache = NULL;     /* cache[n-1] holds a(n) */
static idx_t  cache_size = 0;   /* number of elements stored */
static idx_t  cache_cap   = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'needed' elements */
static void
ensure_cap(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap = new_cap * 2;

    void *tmp = realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        abort();              /* out of memory – cannot recover */
    cache = (seq_t *)tmp;
    cache_cap = new_cap;
}

/* Compute the nth term of A100296. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n<1, but safe to return 0 */
        return (seq_t)0;

    /* initialise first three terms on first call */
    if (cache_size == 0) {
        ensure_cap(3);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)6;   /* a(2) */
        cache[2] = (seq_t)25;  /* a(3) */
        cache_size = 3;
    }

    if (n <= cache_size)
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i < n; ++i) {
        ensure_cap(i + 1);          /* ensure room for cache[i] */
        /* recurrence uses previous three values:
           cache[i-1] = a(i), cache[i-2] = a(i-1), cache[i-3] = a(i-2) */
        seq_t val = (seq_t)4 * cache[i - 1]
                 + (seq_t)2 * cache[i - 2]
                 -          cache[i - 3];
        cache[i] = val;
    }
    cache_size = n;
    return cache[n - 1];
}
