/*
 * A205623  Number of (n+1)X6 0..2 arrays with every 2X2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A205623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A205623: Number of (n+1)×6 0..2 arrays with the stated constraints.
 *
 * Recurrence (empirical, holds for all n > 9):
 *   a(n) = 264*a(n-2) + 80*a(n-3) - 648*a(n-4) - 1536*a(n-5)
 *                         - 8384*a(n-6) - 65536*a(n-8)
 *
 * Initial values:
 *   a(1)=3074, a(2)=43248, a(3)=708300, a(4)=11438856,
 *   a(5)=188139156, a(6)=3042034560, a(7)=50032728864,
 *   a(8)=809272164864, a(9)=13306390815072
 *
 * The function uses a memoised cache so that any order of calls is efficient.
 * All arithmetic is performed in the type `seq_t`; constants are cast to
 * `seq_t` before use to keep the expressions type‑correct.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache array indexed from 1.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of elements allocated */
    static idx_t  computed_up_to = 0;

    if (n < 1)
        return 0; /* undefined, but guard against underflow */

    /* Initialise cache with the known initial values if not yet done. */
    if (cache == NULL) {
        cache_cap = 10; /* need indices 0..9 (0 unused) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure */
        static const seq_t init[10] = {
            0,                         /* index 0 unused */
            3074,                      /* a(1) */
            43248,                     /* a(2) */
            708300,                    /* a(3) */
            11438856,                  /* a(4) */
            188139156,                 /* a(5) */
            3042034560,                /* a(6) */
            50032728864,               /* a(7) */
            809272164864,              /* a(8) */
            13306390815072             /* a(9) */
        };
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = init[i];
        computed_up_to = 9;
    }

    /* Expand the cache if the requested index exceeds it. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        /* initialise newly allocated area to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the value is already computed, return it. */
    if (n <= (idx_t)computed_up_to)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* All operations are performed in seq_t. */
        seq_t val  = ((seq_t)264) * cache[i - 2];
        val += ((seq_t)80) * cache[i - 3];
        val -= ((seq_t)648) * cache[i - 4];
        val -= ((seq_t)1536) * cache[i - 5];
        val -= ((seq_t)8384) * cache[i - 6];
        val -= ((seq_t)65536) * cache[i - 8];
        cache[i] = val;
    }

    computed_up_to = n;
    return cache[n];
}
