/*
 * A209729  1/4 the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 14*a(n-2) + 4*a(n-3); Initial values: a(1)=22,
 *            a(2)=124, a(3)=696
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, exit, size_t */

seq_t a(idx_t n)
{
    /* cache[0] holds a(1), cache[1] holds a(2), … */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */

    if (n == 0)
        return 0;   /* undefined for n<1, but return 0 safely */

    size_t target = (size_t)n;

    if (cache_len < target) {
        /* allocate larger cache */
        size_t new_len = target;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache)          /* allocation failure */
            exit(1);

        /* copy old values */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* fill missing entries using the recurrence */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0)
                new_cache[0] = (seq_t)22;
            else if (i == 1)
                new_cache[1] = (seq_t)124;
            else if (i == 2)
                new_cache[2] = (seq_t)696;
            else
                new_cache[i] =
                    (seq_t)3  * new_cache[i-1] +
                    (seq_t)14 * new_cache[i-2] +
                    (seq_t)4  * new_cache[i-3];
        }

        free(cache);                /* discard old cache */
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n-1];   /* return the requested term */
}
