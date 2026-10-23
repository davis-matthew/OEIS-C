/*
 * A192815  Coefficient of x in the reduction of the polynomial (2*x + 1)^n by
 *          x^3 -> x^2 + x + 1.
 * https://oeis.org/A192815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) + 7*a(n-3) Initial values: a(0) = 0,
 *            a(1) = 2, a(2) = 4 Use the recurrence to compute arbitrary n.
 *            cache[] stores computed values
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"   /* alloc_seq(size_t)  */
#include "../utils/free_seq.h"    /* free_seq(seq_t*)   */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    size_t target = (size_t)n;          /* n is unsigned, fits in size_t */

    /* enlarge the cache if necessary */
    if (target >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= target) new_cap <<= 1;   /* double until big enough */

        seq_t *new_cache = alloc_seq(new_cap);    /* allocate array of seq_t */
        if (!new_cache) abort();                  /* allocation failed */

        /* copy old values */
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        if (cache) free_seq(cache);               /* free old array */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing entries up to target */
    for (size_t i = cache_size; i <= target; ++i) {
        if (i == 0)        cache[i] = 0;
        else if (i == 1)   cache[i] = 2;
        else if (i == 2)   cache[i] = 4;
        else               cache[i] = 5*cache[i-1] - 3*cache[i-2] + 7*cache[i-3];
    }

    cache_size = target + 1;   /* all values up to target are now stored */
    return cache[target];
}
