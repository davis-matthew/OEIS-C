/*
 * A239840  Number of ordered pairs of permutation functions (f,g) on n
 *          elements satisfying f(x) = f(g(g(x))).
 * https://oeis.org/A239840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n*(n-1)^2*a(n-2), a(0) = a(1) = 1 (OEIS
 *            A239840)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last_computed = 0;   /* highest index that has been filled */

    /* Ensure that the cache has room for indices up to n (and at least 0,1). */
    idx_t needed = (n + 1 > 2) ? (n + 1) : 2;      /* at least indices 0 and 1 */
    if (needed > cache_cap) {
        idx_t new_cap;
        if (cache_cap < 2)
            new_cap = needed;
        else
            new_cap = cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;   /* double until enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                       /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialisation of base cases (only once) */
    if (cache == NULL || last_computed < 1) {
        /* cache now points to a block of at least two elements */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* Return known value if already computed */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0)          /* only for completeness */
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else {
            seq_t ui = (seq_t)i;
            seq_t prev = cache[i - 1];
            seq_t prevprev = cache[i - 2];
            seq_t u1 = (seq_t)(i - 1);
            seq_t temp = u1 * u1;     /* (i-1)^2 */
            temp *= prevprev;         /* (i-1)^2 * a(i-2) */
            cache[i] = ui * prev + ui * temp;
        }
    }
    last_computed = n;
    return cache[n];
}
