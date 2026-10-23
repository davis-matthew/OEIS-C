/*
 * A179510  Number of equally-spaced triples (x,y,z) of preceding terms with
 *          x+y=z.
 * https://oeis.org/A179510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179510: a(n) = number of equally–spaced triples (i,j,k) of preceding
   terms with a[i] + a[j] = a[k].  The sequence is computed incrementally:
   a(0) = 0, and for each k>0 the new value is a(k-1) plus the number of
   triples (i,j,k) satisfying the conditions.  The function memoizes the
   values in a static array so that calls in arbitrary order are efficient. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External types supplied by the build environment */
extern seq_t *seq_alloc(size_t); /* may not be used but allowed */

/* Static cache for computed values -------------------------------------- */
static seq_t *cache  = NULL;   /* array of a(i) values */
static size_t cachelen = 0;    /* number of values currently stored */
static size_t cachecap = 0;    /* current capacity of the array */

/* Ensure the cache can hold values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cachecap)
        return;

    size_t newcap = cachecap ? cachecap * 2 : 8;
    if (newcap < needed)
        newcap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cachecap = newcap;
}

/* Main function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise the first term if necessary */
    if (cachelen == 0) {
        ensure_cache(0);
        cache[0] = 0;
        cachelen = 1;
    }

    /* Grow the cache to hold up to index n */
    if ((size_t)(n + 1) > cachecap)
        ensure_cache(n);

    /* Compute any missing values up to n */
    for (idx_t m = cachelen - 1; m < n; ++m) {
        seq_t added = 0;                     /* triples ending at m+1 */
        /* m is the index of the current "k" value */
        if (m >= 2) {
            for (idx_t i = 0; i + 2 <= m; ++i) {
                idx_t diff = m - i;
                if (diff & 1u)         /* (m-i) must be even */
                    continue;
                idx_t d = diff >> 1;    /* spacing between i and j */
                /* d > 0 because m-i >= 2 */
                idx_t j = i + d;
                if (cache[i] + cache[j] == cache[m])
                    ++added;
            }
        }
        cache[m + 1] = cache[m] + added;
    }

    cachelen = (size_t)(n + 1);
    return cache[n];
}
