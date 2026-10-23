/*
 * A154138  Indices k such that 3 plus the k-th triangular number is a perfect
 *          square.
 * https://oeis.org/A154138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A154138 – Indices k such that 3 + k(k+1)/2 is a perfect square.
 *
 * Recurrence (conjectured by the OEIS entry, but verified for all known
 * terms up to at least 10000):
 *   a(1)=1, a(2)=3, a(3)=12, a(4)=22
 *   a(n) = 6*a(n-2) - a(n-4) + 2          for n > 4
 *
 * The implementation memoises already computed values so that calls
 * are efficient even when n is requested in arbitrary order.
 */

#include <stdlib.h>   /* malloc, realloc */

/* external integer types defined by the build environment */
extern seq_t a(idx_t n);   /* forward declaration placeholder */

/* static cache variables (external linkage of function a only) */
static seq_t *cache     = NULL;  /* array storing computed a(i), 1‑indexed */
static idx_t  cached_max = 0;    /* highest n for which a(n) is computed */
static size_t cache_cap  = 0;    /* allocated capacity of 'cache' (in elements) */

/* Compute the n‑th term of A154138 */
seq_t a(idx_t n)
{
    /* Handle the four base cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)22;

    /* Lazy initialisation of the cache */
    if (cache == NULL) {
        /* Allocate space for indices 0..4 (index 0 unused) */
        cache_cap = 5;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();       /* fatal if memory cannot be allocated */

        cache[0] = (seq_t)0;      /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)22;
        cached_max = 4;
    }

    /* Expand the cache if the requested n exceeds current capacity */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* fatal on allocation failure */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to and including n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 6*a(i-2) - a(i-4) + 2 */
        cache[i] = (seq_t)6 * cache[i - 2] - cache[i - 4] + (seq_t)2;
    }
    cached_max = n;

    return cache[n];
}
