/*
 * A095795  a(0)=2, a(1)=5, a(n+2) = a(n+1) + (-1)^n a(n).
 * https://oeis.org/A095795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 5, a(n+2) = a(n+1) + (-1)^n a(n)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS sequence A095795.  The function is
   thread‑local: it caches results in a static array and expands
   the array on demand.  It works for any unsigned 64‑bit index
   and any seq_t that supports the arithmetic operators. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return small terms immediately without allocation */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;

    /* Static cache: array of computed terms and its size */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of allocated slots */
    static size_t last_computed = 0;  /* last index that has a value */

    /* Allocate or grow the cache to hold indices up to n */
    if (cache_len <= n) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();         /* fatal: out of memory */
        cache = tmp;
        cache_len = new_len;
        /* Initialise base values if this is the first allocation */
        if (last_computed < 1) {
            cache[0] = (seq_t)2;
            cache[1] = (seq_t)5;
            last_computed = 1;
        }
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* i-2 is always non‑negative here because i>=2 */
        if (((i - 2) & 1) == 0) {
            /* (+1) * a[i-2] */
            cache[i] = cache[i - 1] + cache[i - 2];
        } else {
            /* (-1) * a[i-2] */
            cache[i] = cache[i - 1] - cache[i - 2];
        }
    }
    last_computed = n;

    return cache[n];
}
