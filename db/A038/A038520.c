/*
 * A038520  Number of elements of GF(2^n) with trace 1 and subtrace 0.
 * https://oeis.org/A038520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-3) for n > 3, a(0)=0, a(1)=1, a(2)=0,
 *            a(3)=3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for previously computed values.  The cache is initialized
 * lazily on the first call and extended as needed. */
static seq_t *cache = NULL;      /* array of seq_t values */
static idx_t cached_max = (idx_t)-1;  /* highest index stored */

/* Return the number of elements of GF(2^n) with trace 1 and subtrace 0. */
seq_t a(idx_t n)
{
    /* ---- Allocate / resize the cache if necessary. ---- */
    if (cache == NULL) {
        /* Ensure space for at least the first four terms. */
        size_t sz = n + 1;
        if (sz < 4) sz = 4;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();          /* should not happen */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)3;
        cached_max = 3;
    } else if (n > cached_max) {
        /* Grow the array to hold all indices up to n. */
        size_t new_sz = (size_t)n + 1;
        if (new_sz > (size_t)cached_max + 1) {
            seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
    }

    /* ---- Return cached value if already computed. ---- */
    if (n <= cached_max)
        return cache[n];

    /* ---- Compute missing values using the recurrence. ---- */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 3) {
            /* Base values already set. */
            continue;
        }
        seq_t term_prev2 = cache[i - 2];
        seq_t term_prev3 = cache[i - 3];
        cache[i] = term_prev2 * (seq_t)2 + term_prev3 * (seq_t)4;
    }
    cached_max = n;
    return cache[n];
}
