/*
 * A144301  a(0) = a(1) = 1; thereafter a(n) = (2*n-3)*a(n-1) + a(n-2).
 * https://oeis.org/A144301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A144301 – a(0) = a(1) = 1;
    a(n) = (2*n-3)*a(n-1) + a(n-2)   for n >= 2   */
/*  Implements memoisation to allow arbitrary order of calls. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;      /* array of terms */
static size_t cache_cap = 0;       /* allocated size of the array   */
static idx_t cache_computed = 0;   /* highest n that has been filled */

/* Ensure that the cache can store terms up to index n.  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp;
#ifdef __cplusplus
    tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp) return; /* allocation failed – should never happen */

    cache = tmp;
    cache_cap = new_cap;
    /* new memory is uninitialised – we will overwrite all required indices below */
}

/*  Return the n‑th term of sequence A144301.  */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(0) and a(1) are 1 */

    ensure_cache(n);

    /* initialise base values if not already done */
    if (cache_computed == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache_computed = 1;
    }

    /* compute missing terms up to n */
    if (cache_computed < n) {
        for (idx_t i = cache_computed + 1; i <= n; ++i) {
            seq_t t1   = cache[i - 1];
            seq_t t2   = cache[i - 2];
            seq_t coeff = (seq_t)(2 * (seq_t)i - 3);
            cache[i] = coeff * t1 + t2;
        }
        cache_computed = n;
    }

    return cache[n];
}
