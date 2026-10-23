/*
 * A032829  Numbers whose set of base-5 digits is {3,4}.
 * https://oeis.org/A032829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=3, a(2)=4; a(n)=5*a(floor(n/2)) + 3 for n odd,
 *            a(n)=5*a(floor((n-1)/2)) + 4 for n even
 * Range:     u8 n <= 14, u16 n <= 190, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) once computed.
       cache[0] is unused; cache[1]=3, cache[2]=4. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    if (n < 1) return 0;          /* undefined below offset – not called. */

    /* Resize the cache if it is too small. */
    if (cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* unrecoverable memory error: abort. */
        cache = tmp;
        /* Initialise any new entries to 0 (meaning “not yet computed”). */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Initialise the special first two terms if necessary. */
    if (n == 1) {
        if (cache[1] == 0) cache[1] = 3;
        return cache[1];
    }
    if (n == 2) {
        if (cache[2] == 0) cache[2] = 4;
        return cache[2];
    }

    /* Return a memoised value if already computed. */
    if (cache[n] != 0) return cache[n];

    /* Recursive computation using the recurrence. */
    seq_t val;
    if (n & 1U) {                      /* odd */
        val = 5 * a(n >> 1) + 3;
    } else {                           /* even */
        val = 5 * a((n >> 1) - 1) + 4;
    }
    cache[n] = val;
    return val;
}
