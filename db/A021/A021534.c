/*
 * A021534  Expansion of 1/((1-x)(1-3x)(1-6x)(1-12x)).
 * https://oeis.org/A021534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 147*a(n-2) + 342*a(n-3) - 216*a(n-4) Initial
 *            values: a(0)=1, a(1)=22, a(2)=337, a(3)=4522
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_seq.h"   /* provides grow_seq() */

static seq_t *cache = NULL;    /* dynamic cache of computed values */
static size_t  cache_cap = 0;  /* current capacity of cache array      */
static idx_t   computed_max = 0;  /* highest index already filled */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough.  Grow_seq handles proper allocation
       and construction for arbitrary‑precision types. */
    if (cache_cap < (size_t)(n + 1))
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));

    /* Initialise the base cases (idempotent – harmless to repeat). */
    cache[0] = (seq_t)1;
    if (n >= 1) cache[1] = (seq_t)22;
    if (n >= 2) cache[2] = (seq_t)337;
    if (n >= 3) cache[3] = (seq_t)4522;

    /* Compute any missing terms up to n.  Skip indices already computed. */
    idx_t start = computed_max + 1;
    if (start < 4)          /* the first recurrence value is a(4) */
        start = 4;

    for (idx_t i = start; i <= n; ++i)
    {
        cache[i] = (seq_t)22 * cache[i - 1]
                 - (seq_t)147 * cache[i - 2]
                 + (seq_t)342 * cache[i - 3]
                 - (seq_t)216 * cache[i - 4];
    }

    if (n > computed_max)
        computed_max = n;

    return cache[n];
}
