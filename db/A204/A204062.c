/*
 * A204062  Expansion of g.f.: exp( Sum_{n>=1} A002203(n)^2 * x^n/n ) where
 *          A002203 are the companion Pell numbers.
 * https://oeis.org/A204062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 10*a(n-2) + 4*a(n-3) - a(n-4) for n >= 4 with
 *            initial values a(0)=1, a(1)=4, a(2)=26, a(3)=148 (derived from
 *            the generating function 1/((1+x)^2*(1-6x+x^2)))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc */

static seq_t *cache = NULL;       /* array of computed terms */
static size_t cache_len = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;      /* current capacity of the array */

/* Grow the cache to hold at least 'needed' terms. */
static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;   /* already sufficient */

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp == NULL)
        exit(EXIT_FAILURE);   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function required by the evaluator */
seq_t a(idx_t n)
{
    /* Convert index to size_t for array access; safe for typical test sizes */
    size_t idx = (size_t)n;
    ensure_capacity(idx + 1);

    /* Compute any missing terms up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)4;
        else if (i == 2)
            cache[i] = (seq_t)26;
        else if (i == 3)
            cache[i] = (seq_t)148;
        else {
            seq_t term = (seq_t)4 * cache[i-1];
            term += (seq_t)10 * cache[i-2];
            term += (seq_t)4 * cache[i-3];
            term -= cache[i-4];
            cache[i] = term;
        }
    }
    cache_len = idx + 1;
    return cache[idx];
}
