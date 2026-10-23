/*
 * A015095  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=10.
 * https://oeis.org/A015095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n+1)=∑_{i=0..n} 10^i * a(i) * a(n-i) (q=10)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>  /* for memset */

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t  cap   = 0;

/* Ensure that the cache holds indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cap   = (size_t)n + 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        memset(cache, 0, cap * sizeof(seq_t));
    } else if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero the newly allocated part */
        memset(tmp + cap, 0, (newcap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }
}

seq_t a(idx_t n)
{
    /* Make sure we have space for up to n. */
    ensure_cache(n);

    /* Initialise the base term a(0)=1 if not already there. */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;

    /* Compute all missing terms up to n. */
    for (idx_t k = 1; k <= n; ++k) {
        if (cache[k] != (seq_t)0)
            continue;                 /* already computed */

        seq_t sum = (seq_t)0;
        seq_t pow = (seq_t)1;          /* 10^0 */

        /* a(k) = Σ_{i=0}^{k-1} 10^i * a(i) * a(k-1-i) */
        for (idx_t i = 0; i < k; ++i) {
            seq_t prod = cache[i] * cache[k-1-i];
            prod *= pow;
            sum += prod;
            pow *= (seq_t)10;
        }
        cache[k] = sum;
    }

    return cache[n];
}
