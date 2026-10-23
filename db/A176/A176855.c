/*
 * A176855  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=1, k=-1 and l=0.
 * https://oeis.org/A176855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 24, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176855
 *   a(0) = 1, a(1) = 1
 *   a(n+1) = Σ_{p=0}^{n} (a(p) * a(n-p) + k) + l
 *   with k = -1, l = 0
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* cache of computed terms */
static seq_t *cache = NULL;        /* array of cached a(n) */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t  last_comp = 0;       /* highest index computed so far (0 at start) */
static bool initialized = false;

/* Ensure the cache can store indices up to n inclusive */
static void grow_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    if (cache_cap == 0)
        cache_cap = 2;            /* at least space for a(0) and a(1) */
    while (cache_cap <= (size_t)n)
        cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    cache = tmp; /* on failure tmp is NULL, propagating */
}

/* Initialise the cache with the first two terms */
static void init_cache(void)
{
    if (initialized)
        return;
    grow_cache(1);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    last_comp = 1;
    initialized = true;
}

/* Compute all terms up to index n (inclusive) using the recurrence */
static void compute_up_to(idx_t n)
{
    if (n <= last_comp)
        return;
    grow_cache(n);
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t m = i - 1;                 /* convolution length - 1 */
        for (idx_t p = 0; p <= m; ++p) {
            sum += cache[p] * cache[m - p] - (seq_t)1;   /* k = -1 */
        }
        cache[i] = sum;
    }
    last_comp = n;
}

/* Public accessor required by the build system */
seq_t a(idx_t n)
{
    init_cache();
    compute_up_to(n);
    return cache[n];
}
