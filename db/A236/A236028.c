/*
 * A236028  Number of (n+1)X(1+1) 0..2 arrays colored with the difference of
 *          the upper median and the lower median in each 2X2 subblock.
 * https://oeis.org/A236028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*a(n-2) + 4*a(n-3) + 192*a(n-4) + 324*a(n-5)
 *            + 696*a(n-6) + 1808*a(n-7) + 1312*a(n-8) + 1152*a(n-9)
 *            + 1024*a(n-10)
 * Initial values (for 1 ≤ n ≤ 10):
 *   81, 384, 1848, 8856, 42724, 205660, 990076,
 *   4760380, 22914572, 110251484
 *
 * This function computes the terms of OEIS A236028 using memoisation.
 * The cache grows only when needed and never shrinks.
 * No external libraries other than the standard C headers are required.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static idx_t cache_size = 0;  /* highest index currently stored */

static void ensure_cache(idx_t n) {
    if (n <= cache_size)                /* already computed */
        return;

    /* The cache must hold indices up to at least 10 when initializing */
    size_t new_cap = (n > 10 ? (size_t)(n + 1) : 11);   /* 1‑based size */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* abort on allocation failure – acceptable for this interface */
        abort();
    }
    cache = tmp;

    /* Initialise base terms once, if the cache was empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)0; /* unused, keeps indices 1..10 valid */
        cache[1]  = (seq_t)81;
        cache[2]  = (seq_t)384;
        cache[3]  = (seq_t)1848;
        cache[4]  = (seq_t)8856;
        cache[5]  = (seq_t)42724;
        cache[6]  = (seq_t)205660;
        cache[7]  = (seq_t)990076;
        cache[8]  = (seq_t)4760380;
        cache[9]  = (seq_t)22914572;
        cache[10] = (seq_t)110251484;
    }

    /* Compute missing terms.  If the cache currently holds fewer than 10
       elements, we start computing from 11 only, as the first ten are known. */
    idx_t start = (cache_size >= 10 ? cache_size + 1 : 11);
    if (start < 11)
        start = 11;
    for (idx_t i = start; i <= n; ++i) {
        seq_t v = (seq_t)0;
        v += (seq_t)9    * cache[i-2];
        v += (seq_t)4    * cache[i-3];
        v += (seq_t)192  * cache[i-4];
        v += (seq_t)324  * cache[i-5];
        v += (seq_t)696  * cache[i-6];
        v += (seq_t)1808 * cache[i-7];
        v += (seq_t)1312 * cache[i-8];
        v += (seq_t)1152 * cache[i-9];
        v += (seq_t)1024 * cache[i-10];
        cache[i] = v;
    }

    if (n > cache_size)
        cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1 */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
