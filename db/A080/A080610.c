/*
 * A080610  Partial sums of Jacobsthal gap sequence.
 * https://oeis.org/A080610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(n) = a(n-1)*4 if n is even, a(n) = a(n-1)+1 if n is
 *            odd
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_n_done = 0;   /* highest n computed so far */

    /* initialise cache with a(0) */
    if (!cache) {
        cache_size = 16;                   /* initial capacity */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failure: return 0 */
        cache[0] = (seq_t)0;
        max_n_done = 0;
    }

    /* grow cache if necessary */
    if (n > max_n_done) {
        if ((idx_t)(n + 1) > cache_size) {
            size_t new_size = cache_size;
            while ((size_t)(n + 1) > new_size) new_size <<= 1;  /* double until enough */
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return (seq_t)0;                          /* allocation failure */
            cache = tmp;
            cache_size = new_size;
        }

        /* compute missing terms, storing them in cache */
        for (idx_t i = max_n_done + 1; i <= n; ++i) {
            if ((i & 1) == 0)                 /* even */
                cache[i] = cache[i - 1] * ((seq_t)4);
            else                              /* odd */
                cache[i] = cache[i - 1] + ((seq_t)1);
        }
        max_n_done = n;
    }

    return cache[n];
}
