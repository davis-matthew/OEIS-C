/*
 * A118161  Start with 1 and repeatedly reverse the digits and add 55 to get
 *          the next term.
 * https://oeis.org/A118161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less, easy
 * Formula:   a(1)=1; a(n)=reverse(a(n-1))+55 (n>1)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse.h"

/* Return a(n) for the sequence A118161.
 * The function memoizes computed values so that
 * calls are efficient even when n comes in any order. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined below the offset */

    /* static cache for previously computed terms */
    static seq_t *cache  = NULL;            /* array indexed from 1 */
    static size_t cap   = 0;                /* current capacity */
    static size_t max   = 0;                /* highest index computed */

    /* initialise cache on first call */
    if (!cache) {
        cap = 4;                            /* initial size */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) exit(1);
        /* a(1) is given */
        cache[1] = 1;
        max = 1;
    }

    /* grow capacity if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;   /* double */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    /* extend cache up to n */
    for (size_t i = max + 1; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse(prev);       /* reverse decimal digits */
        cache[i] = rev + (seq_t)55;
    }
    max = (size_t)n;

    return cache[n];
}
