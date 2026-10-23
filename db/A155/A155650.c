/*
 * A155650  a(n) = 7^n - 6^n + 1.
 * https://oeis.org/A155650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 42*a(n-2) + 30 (a(0)=1, a(1)=2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence is equivalent to a(n) = 7^n - 6^n + 1.  */
#include <stdlib.h>

static seq_t *cache   = NULL;   /* array of previously computed terms   */
static size_t cache_alloc = 0;  /* allocated size of cache             */
static size_t cache_used  = 0;  /* number of computed entries in cache */

static void
ensure_alloc(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > cache_alloc) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(1);             /* abort on allocation failure */
        cache        = tmp;
        cache_alloc  = need;
    }
}

seq_t a(idx_t n)
{
    ensure_alloc(n);

    if (n >= cache_used) {
        /* compute missing terms up to n */
        for (size_t k = cache_used; k <= (size_t)n; ++k) {
            if (k == 0)          cache[k] = (seq_t)1;
            else if (k == 1)      cache[k] = (seq_t)2;
            else
                cache[k] = (seq_t)13 * cache[k-1] - (seq_t)42 * cache[k-2] + (seq_t)30;
        }
        cache_used = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
