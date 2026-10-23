/*
 * A131056  A007318 * A131055.
 * https://oeis.org/A131056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3) for n >= 5 Initial values:
 *            a(1)=1, a(2)=3, a(3)=7, a(4)=17
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memcpy (optional) */

seq_t a(idx_t n)
{
    /* cache[1] corresponds to a(1); cache[0] is unused */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* allocated size (bytes) */
    static size_t  sz     = 0;   /* greatest index computed */

    if (n < 1)          /* offset is 1; undefined below 1 but guard */
        return 0;

    /* Allocate / extend cache if necessary */
    if (cap < (n + 1) * sizeof(seq_t)) {
        size_t new_cap = cap > 0 ? cap * 2 : 64;   /* bytes */
        while (new_cap < (n + 1) * sizeof(seq_t))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)malloc(new_cap);
        if (!tmp) abort();                 /* out of memory */
        if (cache) {
            memcpy(tmp, cache, (sz + 1) * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first few values if not yet present */
    if (sz < 1) {
        cache[1] = (seq_t)1;
        sz = 1;
    }
    if (sz < 2) {
        cache[2] = (seq_t)3;
        sz = 2;
    }
    if (sz < 3) {
        cache[3] = (seq_t)7;
        sz = 3;
    }
    if (sz < 4) {
        cache[4] = (seq_t)17;
        sz = 4;
    }

    /* Compute missing terms up to n */
    for (idx_t i = sz + 1; i <= n; ++i) {
        /* a(i) = 5*a(i-1) - 8*a(i-2) + 4*a(i-3) */
        cache[i] = (seq_t)5 * cache[i-1]
                 - (seq_t)8 * cache[i-2]
                 + (seq_t)4 * cache[i-3];
    }
    sz = n;

    return cache[n];
}
