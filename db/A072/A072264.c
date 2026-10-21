/*
 * A072264  a(n) = 3*a(n-1) + 5*a(n-2), with a(0)=1, a(1)=1.
 * https://oeis.org/A072264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+5*a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */
#include <stddef.h>   /* size_t */

/* The build supplies seq_t and idx_t.
   This function is not static so it has external linkage. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *cache = NULL;      /* array of terms */
    static idx_t   size  = 0;        /* number of terms stored */
    static idx_t   cap   = 0;        /* allocated capacity */

    /* Ensure capacity big enough for index n */
    if (n + 1 > cap) {
        /* double capacity until it fits the required index */
        idx_t new_cap = cap ? cap : 4;
        while (new_cap <= n)
            new_cap <<= 1;     /* multiply by 2 */

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* on allocation failure return 0 (should not happen in tests) */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize first two terms if cache is empty */
    if (size == 0) {
        cache[0] = 1;
        cache[1] = 1;
        size = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = 1;                /* header guarantees this only once */
        } else {
            /* recurrence: a(i) = 3*a(i-1) + 5*a(i-2) */
            cache[i] = 3 * cache[i-1] + 5 * cache[i-2];
        }
    }
    size = n + 1;  /* all terms up to n are now in cache */

    return cache[n];
}
