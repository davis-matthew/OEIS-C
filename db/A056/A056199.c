/*
 * A056199  a(n) = n * a(n-1) - Sum_{k=1..n-2} a(k) with a(1) = 0 and a(2) =
 *          1.
 * https://oeis.org/A056199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=3, a(n) = (n+1)*a(n-1) - n*a(n-2) for n>=4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* array indexed by n (1‑based) */
    static idx_t cached_max = 0;     /* largest n already computed */
    static size_t cache_cap = 0;     /* allocated array size (>= cached_max+1) */

    /* If the value was already computed, return it immediately */
    if (n <= cached_max)
        return cache[n];

    /* Ensure enough space in the cache for indices up to n */
    size_t need = (size_t)(n + 1);   /* +1 to leave index 0 unused */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first three terms if they have not yet been stored */
    if (cached_max < 1) {
        /* a(1) = 0   */
        cache[1] = (seq_t)0;
        cached_max = 1;
    }
    if (cached_max < 2) {
        /* a(2) = 1   */
        cache[2] = (seq_t)1;
        cached_max = 2;
    }
    if (cached_max < 3) {
        /* a(3) = 3   */
        cache[3] = (seq_t)3;
        cached_max = 3;
    }

    /* Compute any missing terms up to n using the linear recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)0;
        } else if (i == 2) {
            val = (seq_t)1;
        } else if (i == 3) {
            val = (seq_t)3;
        } else {
            /* (i+1) * a(i-1) - i * a(i-2) */
            seq_t term1 = ((seq_t)(i + 1)) * cache[i - 1];
            seq_t term2 = ((seq_t)i) * cache[i - 2];
            val = term1 - term2;
        }
        cache[i] = val;
    }

    cached_max = n;
    return cache[n];
}
