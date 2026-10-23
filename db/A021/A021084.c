/*
 * A021084  Expansion of 1/((1-x)(1-2x)(1-4x)(1-9x)).
 * https://oeis.org/A021084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=16, a(2)=179, a(3)=1766;
   a(n)=16*a(n-1)-77*a(n-2)+134*a(n-3)-72*a(n-4)  for n>3 */
#include <stdlib.h>

static seq_t *cache = NULL;        /* memoization array */
static idx_t  cache_cap = 0;       /* capacity of the array (elements) */
static idx_t  last_computed = 0;   /* largest n computed so far */

static const seq_t init_vals[4] = {1, 16, 179, 1766};

seq_t a(idx_t n)
{
    /* Base cases handled directly. */
    if (n < 4) return init_vals[n];

    /* Allocate or grow the cache if necessary. */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                     /* allocation failure */
        /* store the initial 4 values */
        for (idx_t i = 0; i < 4; ++i) cache[i] = init_vals[i];
        last_computed = 3;
    } else if (cache_cap <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = 16 * t1 - 77 * t2 + 134 * t3 - 72 * t4;
    }
    last_computed = n;
    return cache[n];
}
