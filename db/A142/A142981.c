/*
 * A142981  a(1) = 1, a(2) = 7, a(n+2) = 7*a(n+1) + (n+1)^2*a(n).
 * https://oeis.org/A142981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=7,
   a(n+2)=7*a(n+1)+(n+1)^2*a(n)  for n>=1
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused offset */

    /* Handle the first two terms directly */
    if (n == 1) return 1;
    if (n == 2) return 7;

    /* Cache of already computed values (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size of the array   */
    static idx_t  last_computed = 2;  /* highest index already computed */

    /* initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = 3;                     /* indices 0..2 will be valid */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure */
        cache[1] = 1;
        cache[2] = 7;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t = (seq_t)(i - 1);          /* (i-1) as seq_t */
        cache[(size_t)i] = cache[(size_t)(i - 1)] * 7
                         + cache[(size_t)(i - 2)] * t * t;
    }

    last_computed = n;
    return cache[(size_t)n];
}
