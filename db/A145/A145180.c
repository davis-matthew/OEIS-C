/*
 * A145180  Continued cotangent recurrence a(n+1) = a(n)^3 + 3*a(n) and a(1) =
 *          6.
 * https://oeis.org/A145180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n)^3 + 3*a(n), a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoised recurrence, works for any seq_t that supports the
   operators +, *, and assignment.  The cache is dimensioned
   as a 1‑based array of computed terms, expanded with realloc. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* 1‑based array of terms  */
static idx_t cache_len = 0;   /* largest index that has been computed */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;                /* already computed */

    /* Expand the cache to size n+1 (index 0 unused) */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);                         /* out of memory */
    cache = tmp;

    /* Initialise the very first term if this is the first call */
    if (cache_len == 0) {
        cache[1] = (seq_t)6;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val  = prev * prev * prev + (seq_t)3 * prev;
        cache[i] = val;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but not used */
    ensure_cache(n);
    return cache[n];
}
