/*
 * A016207  Expansion of g.f. 1/((1-x)*(1-2*x)*(1-12*x)).
 * https://oeis.org/A016207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=15, a(2)=187,
   a(n) = 15*a(n-1) - 38*a(n-2) + 24*a(n-3)   for n≥3   (OEIS A016207) */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms */
static idx_t  cache_len = 0;    /* number of entries currently stored (next free index) */
static size_t cache_cap = 0;    /* allocated capacity (in number of elements) */

/* Grow the cache so that it can hold indices 0 … n inclusive. */
static void
ensure_capacity(idx_t n)
{
    if (cache_cap > n)      /* already enough space */
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap *= 2;      /* exponential growth */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);           /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure the cache can hold at least index n. */
    if (!cache) {                  /* first call */
        ensure_capacity(n);
    } else {
        ensure_capacity(n);
    }

    /* Compute missing terms up to n. */
    while (cache_len <= n) {
        switch (cache_len) {
            case 0: cache[0] = 1;                     break;
            case 1: cache[1] = 15;                    break;
            case 2: cache[2] = 187;                   break;
            default: {
                /* a(k) = 15*a(k-1) - 38*a(k-2) + 24*a(k-3) */
                seq_t t1 = 15 * cache[cache_len - 1];
                seq_t t2 = 38 * cache[cache_len - 2];
                seq_t t3 = 24 * cache[cache_len - 3];
                cache[cache_len] = t1 - t2 + t3;
            }
        }
        ++cache_len;
    }

    return cache[n];
}
