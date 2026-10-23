/*
 * A155041  Diagonal sums of symmetric (1,-1)-triangle A155040.
 * https://oeis.org/A155041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=0, a(4)=1 Recurrence:
 *            a(n)=a(n-2)-a(n-3)+a(n-5) for n>=5
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence also satisfies a(2n)=A008611(n+2) and
   a(2n+1)=[x^n](1-x-x^2)/(1-x-x^3+x^4) but the simple
   5‑term recurrence is used for efficient computation. */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_cap = 0;       /* current capacity of cache */

/* Extend cache so that it contains a(0)…a(n)  */
static void extend_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;                   /* already present */

    /* Reallocate (preserving existing entries) */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();            /* allocation failure */
    cache = tmp;

    /* Compute any newly required terms */
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 2;
        } else if (i == 3) {
            cache[i] = 0;
        } else if (i == 4) {
            cache[i] = 1;
        } else {
            /* recurrence: a(n) = a(n-2) - a(n-3) + a(n-5) */
            cache[i] = cache[i-2] - cache[i-3] + cache[i-5];
        }
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
