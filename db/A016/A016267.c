/*
 * A016267  Expansion of 1/((1-x)(1-10x)(1-12x)).
 * https://oeis.org/A016267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (54*12^(n+1) - 55*10^(n+1) + 1)/99 Recurrence: a(0)=1,
 *            a(1)=23, a(n) = 22*a(n-1) - 120*a(n-2) + 1 (n≥2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of computed terms          */
    static idx_t cache_cap = 0;       /* allocated capacity              */
    static idx_t cache_len = 0;       /* number of terms actually set    */

    /* Ensure capacity for at least n+1 elements */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 2;           /* start with space for two terms   */
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Initialize base terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache_len = 2;
    }

    /* If the requested term is already available, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t = (seq_t)22 * cache[i-1];
        t -= (seq_t)120 * cache[i-2];
        t += (seq_t)1;
        cache[i] = t;
    }
    cache_len = n + 1;
    return cache[n];
}
