/*
 * A016302  Expansion of 1/((1-2*x)*(1-5*x)*(1-12*x)).
 * https://oeis.org/A016302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*2^n - 125*5^n + 216*12^n)/105
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=19, a(2)=267, a(n)=19*a(n-1)-94*a(n-2)+120*a(n-3) for n>2 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of cached values   */
    static idx_t cache_sz = 0;    /* number of cached entries */

    /* Grow the cache if necessary */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed   */
        cache = tmp;

        /* Compute missing terms */
        for (idx_t i = cache_sz; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)19;
            else if (i == 2)
                cache[i] = (seq_t)267;
            else {
                seq_t v1 = cache[i-1];
                seq_t v2 = cache[i-2];
                seq_t v3 = cache[i-3];
                cache[i] = (seq_t)19 * v1 - (seq_t)94 * v2 + (seq_t)120 * v3;
            }
        }
        cache_sz = new_sz;
    }

    return cache[n];
}
