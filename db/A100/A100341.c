/*
 * A100341  Denominators of the convergents in the continued fraction
 *          expansion for the constant given by A100338, where the partial
 *          quotients equal A006519 (greatest power of 2 dividing n).
 * https://oeis.org/A100341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2_v2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1)*A006519(n) + a(n-2), a(1)=1, a(2)=2
   A006519(n) = greatest power of 2 dividing n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2_v2.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* size allocated */
    static idx_t max_computed = 0; /* maximum n computed so far */

    /* initialise cache once */
    if (cache == NULL) {
        cache_size = 3;                 /* indices 0,1,2 will be used */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure: return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        max_computed = 2;
    }

    /* if the requested n is already known, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* resize cache if needed */
    if ((idx_t)cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = pow2_v2(i);
        cache[i] = cache[i - 1] * v + cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
