/*
 * A164543  a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 1, a(1) = 17.
 * https://oeis.org/A164543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2); a(0)=1, a(1)=17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised recursion cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* allocated size */
    static idx_t last_computed = 0;  /* highest index computed so far */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = (n < 2 ? 2 : n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;               /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        last_computed = 1;
    }
    /* Grow cache if needed */
    else if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)10;
        seq_t t2 = cache[i - 2] * (seq_t)17;
        cache[i] = t1 - t2;
    }
    if (n > last_computed) last_computed = n;

    return cache[n];
}
