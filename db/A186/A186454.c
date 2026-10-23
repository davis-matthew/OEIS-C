/*
 * A186454  Number of (n+1) X 2 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) (n > 8) Initial
 *            values: a(1)=16, a(2)=21, a(3)=42, a(4)=61, a(5)=106, a(6)=151,
 *            a(7)=245, a(8)=359
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 47, u64 n <= 100, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build RSS: recurrence with memoisation, safe for all seq_t implementations */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for 1 .. 8 */
    static const seq_t base[9] = {
        0, 16, 21, 42, 61, 106, 151, 245, 359
    };

    /* trivial case: small indices */
    if (n <= 8) {
        return base[(idx_t)n];
    }

    /* static cache for computed terms (1‑indexed) */
    static seq_t *cache = NULL;          /* array of size cache_size */
    static idx_t  cache_size   = 0;      /* allocated size (>= n+1 when needed) */
    static idx_t  computed_up = 0;       /* highest index that has a value stored */

    /* ensure we have enough storage */
    if (cache_size < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
        cache_size = n + 1;

        /* initialise the base values if this is the first allocation */
        if (computed_up < 8) {
            for (idx_t i = 1; i <= 8; ++i) {
                cache[i] = base[i];
            }
            computed_up = 8;
        }
    }

    /* compute any pending terms up to n */
    idx_t start = computed_up + 1;
    if (start < 9) start = 9;          /* recurrence only defined for n > 8 */

    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = cache[i-1] + cache[i-1];   /* 2*a(i-1) */
        seq_t t2 = cache[i-3] + cache[i-3];   /* 2*a(i-3) */
        seq_t t3 = cache[i-4] + cache[i-4];   /* 2*a(i-4) */
        seq_t t5 = cache[i-5];                /* a(i-5)   */
        cache[i] = t1 - t2 + t3 - t5;          /* recurrence */
    }

    computed_up = n;
    return cache[n];
}
