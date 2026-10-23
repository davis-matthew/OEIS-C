/*
 * A190005  a(n) = 6*a(n-1) + 10*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 10*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values */
    static size_t cap   = 0;      /* current allocation size (elements) */
    static size_t len   = 0;      /* number of entries already computed */

    /* Immediate return for the base cases */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    /* Lazily initialise the cache on the first call that needs it */
    if (len == 0) {
        cap = 2;                     /* we need at least space for a(0) and a(1) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed – return 0 as a safe fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;                     /* we have computed indices 0 and 1 */
    }

    /* Grow the cache if necessary */
    if ((idx_t)cap <= n) {
        size_t old_cap = cap;
        /* Double until we have enough space for index n */
        while ((idx_t)cap <= n) { cap <<= 1; }
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed – return 0 */
            free(cache);
            cache = NULL;
            cap = 0;
            len  = 0;
            return (seq_t)0;
        }
        cache = tmp;
        /* Newly allocated memory is uninitialised; we will fill it below */
        (void)old_cap;
    }

    /* Compute the sequence up to n if not already done */
    for (idx_t i = len; i <= n; ++i) {
        size_t idx   = (size_t)i;
        size_t im1   = idx - 1;
        size_t im2   = idx - 2;
        cache[idx] = (seq_t)6 * cache[im1] + (seq_t)10 * cache[im2];
    }
    len = (size_t)(n + 1);   /* all indices up to n are now computed */

    return cache[(size_t)n];
}
