/*
 * A092550  Expansion of -x*(1+x+x^2+x^4)/(-1+2*x^3+x^6).
 * https://oeis.org/A092550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = a(3) = 1 a(n) = a(n-1) + a(n-2) if n % 3 != 0 a(n)
 *            = a(n-2) if n % 3 == 0 (for n >= 4) (OEIS A092550, expansion of
 *            -x*(1+x+x^2+x^4)/(-1+2*x^3+x^6))
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed values */
    static seq_t *cache     = NULL;
    static idx_t cache_alloc = 0;   /* allocated size */
    static idx_t cache_size  = 0;   /* highest index computed */

    /* initialise cache once */
    if (cache_alloc == 0) {
        cache_alloc = 4;                 /* indices 0..3 */
        cache = (seq_t*)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* abort on allocation failure */
        cache[0] = (seq_t)0;             /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_size = 3;
    }

    /* extend cache up to n if necessary */
    while (cache_size < n) {
        idx_t i = cache_size + 1;

        /* grow storage if needed */
        if (i >= cache_alloc) {
            idx_t new_alloc = cache_alloc ? cache_alloc << 1 : 4;
            while (new_alloc <= i) new_alloc <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cache_alloc = new_alloc;
        }

        /* recurrence */
        if (i % 3 == 0) {
            cache[i] = cache[i - 2];
        } else {
            cache[i] = cache[i - 1] + cache[i - 2];
        }

        cache_size = i;
    }

    return cache[n];
}
