/*
 * A238907  Number of (n+1) X (2+1) 0..2 arrays with no element equal to all
 *          horizontal neighbors or equal to all vertical neighbors, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A238907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) +32*a(n-2) +8*a(n-3) -51*a(n-4) -43*a(n-5)
 *            -5*a(n-6) +37*a(n-7) -5*a(n-8) -2*a(n-9) +4*a(n-10) Initial
 *            values: a(1)=9, a(2)=41, a(3)=486, a(4)=4287, a(5)=41165,
 *            a(6)=385632, a(7)=3638773, a(8)=34262775, a(9)=322817734,
 *            a(10)=3040984385
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against requests below the offset */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoisation – enough for all indices ever requested */
    static seq_t *cache = NULL;
    static idx_t  cached_limit = 0;   /* largest n for which cache[n] is valid */

    /* The first 10 terms – stored explicitly because they are needed to
       kick‑start the recurrence.  cache[0] is never used.                */
    static const seq_t init[11] = {
        (seq_t)0,          /* dummy for index 0 */
        (seq_t)9,
        (seq_t)41,
        (seq_t)486,
        (seq_t)4287,
        (seq_t)41165,
        (seq_t)385632,
        (seq_t)3638773,
        (seq_t)34262775,
        (seq_t)322817734,
        (seq_t)3040984385
    };

    /* Initialise the cache on the first call.  The cache is sized to hold
       the first 10 terms; larger terms are added lazily as needed.       */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 11);
        if (cache == NULL)
            return (seq_t)0;        /* allocation failure – return 0 */

        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = init[i];

        cached_limit = 10;
    }

    /* If the value is already computed, return it immediately.          */
    if (n <= cached_limit)
        return cache[n];

    /* Grow the cache to accommodate the new request.                      */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (tmp == NULL)
        return (seq_t)0;                          /* allocation failure */

    cache = tmp;

    /* Compute all missing terms up to n using the recurrence.           */
    for (idx_t i = cached_limit + 1; i <= n; ++i) {
        seq_t term = (seq_t)0;
        term += (seq_t)6  * cache[i - 1];
        term += (seq_t)32 * cache[i - 2];
        term += (seq_t)8  * cache[i - 3];
        term -= (seq_t)51 * cache[i - 4];
        term -= (seq_t)43 * cache[i - 5];
        term -= (seq_t)5  * cache[i - 6];
        term += (seq_t)37 * cache[i - 7];
        term -= (seq_t)5  * cache[i - 8];
        term -= (seq_t)2  * cache[i - 9];
        term += (seq_t)4  * cache[i - 10];
        cache[i] = term;
    }

    cached_limit = n;
    return cache[n];
}
