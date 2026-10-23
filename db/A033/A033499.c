/*
 * A033499  a(n) = a(n-1) + a(round(2*(n-1)/3)) + a(round((n-1)/3)) with
 *          a(1)=a(2)=1.
 * https://oeis.org/A033499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(round(2*(n-1)/3)) + a(round((n-1)/3)), a(1) =
 *            a(2) = 1
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 237, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches previously computed terms in a static array.
   The recurrence only refers to indices less than n, so a simple
   forward iteration from 1 up to the requested n guarantees correct
   results regardless of call order.  */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;        /* memoized values, indexed from 1 */
static idx_t cache_cap = 0;       /* allocated size (including index 0)    */
static idx_t computed_upto = 0;   /* highest n for which cache[i] is valid */

static void
ensure_capacity(idx_t n)
{
    if (cache_cap < n + 1) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out‑of‑memory: terminate */
        /* zero the new entries; values != 0 are considered computed */
        if (new_cap > cache_cap)
            memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1, but harmless */

    /* initialise cache on first call */
    if (!cache) {
        ensure_capacity(2);
        cache[1] = 1;
        cache[2] = 1;
        computed_upto = 2;
    }

    if (n <= computed_upto)            /* value already computed */
        return cache[n];

    ensure_capacity(n);

    /* compute missing terms in strictly increasing order */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        idx_t k = i - 1;
        idx_t r2 = (2 * k + 1) / 3;          /* round(2*k/3)  */
        idx_t r1 = (k + 1) / 3;              /* round(k/3)    */
        cache[i] = cache[i - 1] + cache[r2] + cache[r1];
    }
    computed_upto = n;
    return cache[n];
}
