/*
 * A190981  a(n) = 9*a(n-1) - 4*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 4*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static idx_t size   = 0;      /* number of filled entries */
    static idx_t cap    = 0;      /* allocated capacity */

    /* first call: initialise cache with the two initial terms */
    if (cache == NULL) {
        cap = 4;                  /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure – not expected */
        cache[0] = 0;
        cache[1] = 1;
        size = 2;
    }

    /* if requested index already computed, return it immediately */
    if (n < size)
        return cache[n];

    /* ensure enough space for n+1 entries */
    if ((idx_t)(n + 1) > cap) {
        /* grow capacity: either double or enough for n+1, whichever is larger */
        idx_t new_cap = (cap * 2 > (idx_t)(n + 1)) ? cap * 2 : (idx_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure – not expected */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing entries up to index n */
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = 9 * cache[i - 1] - 4 * cache[i - 2];
    }
    size = n + 1;

    return cache[n];
}
