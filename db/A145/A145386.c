/*
 * A145386  a(1) = 1; a(n) = a(n-1)*(2*(n-1)+a(n-1)) for n > 1.
 * https://oeis.org/A145386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; For n > 1, a(n) = a(n-1) * (2*(n-1) + a(n-1))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence undefined for n = 0 */
        return 0;

    static seq_t *cache = NULL;      /* array of already computed terms */
    static idx_t  last = 0;          /* highest n for which a(n) is stored */
    static size_t cap   = 0;          /* allocated capacity of cache */

    /* Expand cache if necessary */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                     /* allocation failure */
            return 0;
        /* initialise the newly allocated part to zero */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n that are not yet known */
    if (last == 0) {                    /* cache is empty */
        cache[1] = 1;
        last = 1;
    }
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev    = cache[i - 1];
        seq_t addend  = (seq_t)(2 * (i - 1)) + prev;
        cache[i] = prev * addend;
    }
    last = n;

    return cache[n];
}
