/*
 * A177142  Expansion of
 *          x*(1+x)*(1+5*x-8*x^2)/(1-5*x-30*x^2+69*x^3+31*x^4-22*x^5).
 * https://oeis.org/A177142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=11, a(3)=82, a(4)=663, a(5)=4985
 *            a(n)=5*a(n-1)+30*a(n-2)-69*a(n-3)-31*a(n-4)+22*a(n-5) for n≥6
 *            (OEIS A177142)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;     /* 1‑based array, index 0 unused */
static idx_t cache_max = 0;     /* largest n for which cache has a value */

/* compute the n‑th term (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of the offset – harmless */

    /* reallocate cache if needed */
    if (n > cache_max) {
        size_t new_size = (size_t)(n + 1);          /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                         /* allocation failure */
        cache = tmp;

        /* generate values from cache_max+1 up to n */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 1)       cache[i] = (seq_t)1;
            else if (i == 2)  cache[i] = (seq_t)11;
            else if (i == 3)  cache[i] = (seq_t)82;
            else if (i == 4)  cache[i] = (seq_t)663;
            else if (i == 5)  cache[i] = (seq_t)4985;
            else {
                /* use 128‑bit intermediate to avoid overflow before cast */
                unsigned __int128 val =
                        (__int128)cache[i-1] * 5  +
                        (__int128)cache[i-2] * 30 -
                        (__int128)cache[i-3] * 69 -
                        (__int128)cache[i-4] * 31 +
                        (__int128)cache[i-5] * 22;
                cache[i] = (seq_t)val;
            }
        }
        cache_max = n;
    }

    return cache[n];
}
