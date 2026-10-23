/*
 * A140160  a(1)=1, a(n) = a(n-1) + n^4 if n odd, a(n) = a(n-1) + n^3 if n is
 *          even.
 * https://oeis.org/A140160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + n^4 if n is odd, a(n) = a(n-1) + n^3
 *            if n is even. (OEIS A140160)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build; do not redefine them. */

seq_t a(idx_t n)
{
    /* Cache for previously computed values: cache[0] == a(1),
       cache[1] == a(2), etc. */
    static seq_t *cache = NULL;
    static idx_t   max_n = 0;   /* number of elements currently cached */

    /* Base case: initialise cache for a(1) */
    if (max_n == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* unlikely, but sanity check */
        cache[0] = (seq_t)1;           /* a(1) = 1 */
        max_n = 1;
    }

    /* If the requested n is beyond the cached range, extend the cache */
    if (n > max_n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failure fallback */
        cache = new_cache;

        /* Compute a(i) for i = max_n+1 .. n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];        /* a(i-1), valid since i >= 2 */
            seq_t base = (seq_t)i;
            seq_t term;

            if (i & 1) { /* i odd: add i^4 */
                term = base * base;
                term *= base;
                term *= base; /* now base^4 */
            } else {     /* i even: add i^3 */
                term = base * base;
                term *= base; /* now base^3 */
            }

            cache[i - 1] = prev + term;        /* store a(i) */
        }

        max_n = n;
    }

    return cache[n - 1];
}
