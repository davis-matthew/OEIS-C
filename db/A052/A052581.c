/*
 * A052581  E.g.f. (1-x)/(1-x-x^4).
 * https://oeis.org/A052581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1,  a(1)=a(2)=a(3)=0;
   for n≥0:  a(n+4) = (n+4)*a(n+3) + (n+1)(n+2)(n+3)(n+4)*a(n)  */
#include <stdlib.h>

static seq_t *cache = NULL;            /* cache of computed terms   */
static idx_t  cache_cap = 0;           /* current capacity of cache */

/* Expand the cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;                  /* already sufficient */

    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);                    /* allocation failed */

    cache = new_cache;

    /* initialise first three terms once */
    if (cache_cap == 0) {
        cache[0] = (seq_t)1;
        cache[1] = cache[2] = cache[3] = (seq_t)0;
    }

    /* compute only the new indices that need the recurrence */
    idx_t start = cache_cap;
    if (start < 4) start = 4;
    for (idx_t i = start; i < new_cap; ++i) {
        idx_t m = i - 4;                       /* use a(m), a(m+3)  */
        seq_t term1 = (seq_t)(m + 4) * cache[m + 3];
        seq_t term2 = (seq_t)(m + 1) * (seq_t)(m + 2) *
                      (seq_t)(m + 3) * (seq_t)(m + 4) * cache[m];
        cache[i] = term1 + term2;
    }

    cache_cap = new_cap;
}

/* Public function that computes a(n) */
seq_t a(idx_t n)
{
    if (n < 4) {
        if (n == 0) return (seq_t)1;
        return (seq_t)0;
    }

    ensure_cache(n);
    return cache[n];
}
