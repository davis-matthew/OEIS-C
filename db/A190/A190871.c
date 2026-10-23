/*
 * A190871  a(n) = 11*a(n-1) - 11*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A190871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 11*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* static cache for memoisation */
    static seq_t *cache = NULL; /* pointer to array of seq_t */
    static idx_t cap   = 0;     /* size of allocated array */
    static idx_t last  = 1;     /* largest index computed so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 2;                 /* store a(0) and a(1) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        last = 1;
    }

    /* enlarge cache if necessary */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute new elements up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term1 = 11 * cache[i-1];
        seq_t term2 = 11 * cache[i-2];
        cache[i] = term1 - term2;
    }
    last = n;

    return cache[n];
}
