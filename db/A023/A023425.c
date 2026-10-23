/*
 * A023425  Generalized Catalan numbers: a(0) = 1, a(n) = a(n-1) +
 *          Sum_{k=1..n-4} a(k) * a(n-k).
 * https://oeis.org/A023425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=a(4)=1; a(n)=a(n-1)+∑_{k=1}^{n-4}
 *            a(k)*a(n-k) for n≥5 (This is the recurrence from OEIS A023425.)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 26, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache storing computed values */
    static seq_t *cache   = NULL;   /* array of a(i) values */
    static size_t  cap   = 0;       /* allocated length of cache  */
    static size_t  next  = 0;       /* first index with no value yet */

    /* Ensure sufficient capacity for index n */
    if (cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;  /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */

        cache = tmp;
        /* initialise new elements to zero (safe for all included types) */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Compute all terms from 'next' up to and including n */
    for (size_t i = next; i <= (size_t)n; ++i) {
        if (i <= 4) {
            cache[i] = 1;   /* base cases */
        } else {
            seq_t sum = 0;
            idx_t maxk = (idx_t)i - 4;          /* k runs 1..i-4 */
            for (idx_t k = 1; k <= maxk; ++k) {
                sum += cache[(size_t)k] * cache[(size_t)(i - k)];
            }
            cache[i] = cache[(size_t)(i - 1)] + sum;
        }
    }
    next = n + 1;   /* all indices up to n are now initialized */

    return cache[(size_t)n];
}
