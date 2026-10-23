/*
 * A116823  Number of permutations of length n which avoid the patterns 1342,
 *          3142, 4312; or avoid the patterns 3124, 3412, 3421.
 * https://oeis.org/A116823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n ≥ 5 a(n) = 7*a(n-1) - 15*a(n-2) + 10*a(n-3) - 2*a(n-4),
 *            with initial values a(1)=1, a(2)=2, a(3)=6, a(4)=21. This
 *            follows from the rational generating function
 *            A(x)=-(x^3-7x^2+5x-1)x/((2x^2-4x+1)(x^2-3x+1))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoised values (1‑based; index 0 unused) */
static seq_t *cache = NULL;   /* cache[1] = a(1), etc. */
static idx_t cache_size = 0;  /* largest n that has been computed */

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; n=0 is undefined */
        return (seq_t)0;

    /* return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* enlarge cache to hold up to n */
    size_t new_cap = (size_t)n + 1;            /* indices 0..n */
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return (seq_t)0;   /* allocation failed (unlikely for n≤10000) */

    /* copy existing cached values */
    for (idx_t i = 1; i <= cache_size; ++i)
        new_cache[i] = cache[i];

    /* compute missing values */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)           new_cache[i] = (seq_t)1;
        else if (i == 2)      new_cache[i] = (seq_t)2;
        else if (i == 3)      new_cache[i] = (seq_t)6;
        else if (i == 4)      new_cache[i] = (seq_t)21;
        else {
            new_cache[i] =
                (seq_t)7 * new_cache[i-1]
              - (seq_t)15 * new_cache[i-2]
              + (seq_t)10 * new_cache[i-3]
              - (seq_t)2  * new_cache[i-4];
        }
    }

    /* replace old cache */
    free(cache);
    cache = new_cache;
    cache_size = n;

    return cache[n];
}
