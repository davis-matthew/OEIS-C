/*
 * A037646  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1.
 * https://oeis.org/A037646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base-4 number formed by the first n terms of the periodic
 *            digit sequence 3,0,1,3,0,1,... (a(1)=3, a(2)=12, a(3)=49, …)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;            /* array storing computed values: a[1..] */
static size_t cache_len = 0;           /* highest index computed */
static size_t cache_cap = 0;           /* allocated capacity (>= cache_len+1) */
static const seq_t digits[3] = { (seq_t)3, (seq_t)0, (seq_t)1 }; /* 3,0,1 cycle */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out‑of‑bounds, not used by callers */

    /* Ensure the cache is large enough for indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* malloc failure → terminate */
        cache = tmp;
        cache_cap = new_cap;
        /* initialise first entry for convenience */
        if (cache_len == 0) cache[0] = (seq_t)0;
    }

    /* Compute values up to n if necessary */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t d    = digits[(i - 1) % 3];
        cache[i] = prev * 4 + d;
    }
    cache_len = (size_t)n;

    return cache[(size_t)n];
}
