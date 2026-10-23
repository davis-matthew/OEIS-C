/*
 * A198405  Number of 2n X 2 0..4 arrays with values 0..4 introduced in row
 *          major order and each element equal to exactly one horizontal and
 *          vertical neighbor.
 * https://oeis.org/A198405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 51*a(n-1) - 691*a(n-2) + 3601*a(n-3) - 7056*a(n-4) +
 *            4096*a(n-5) with initial values a(1)=2, a(2)=17, a(3)=377,
 *            a(4)=11473, a(5)=375273
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* undefined for n<1, but keep a sane return value */
    if (n == 0) return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* enlarge cache if necessary */
    if (n > cache_size)
    {
        /* allocate new cache */
        seq_t *new_cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */

        /* copy old values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* fill missing entries */
        for (idx_t i = cache_size; i < n; ++i)
        {
            if (i < 5) /* base cases (zero based) */
            {
                switch (i) {
                case 0: new_cache[i] = (seq_t)2;   break;
                case 1: new_cache[i] = (seq_t)17;  break;
                case 2: new_cache[i] = (seq_t)377; break;
                case 3: new_cache[i] = (seq_t)11473; break;
                case 4: new_cache[i] = (seq_t)375273; break;
                }
            }
            else /* recurrence */
            {
                new_cache[i] =
                    ((seq_t)51   * new_cache[i-1])
                  - ((seq_t)691  * new_cache[i-2])
                  + ((seq_t)3601 * new_cache[i-3])
                  - ((seq_t)7056 * new_cache[i-4])
                  + ((seq_t)4096 * new_cache[i-5]);
            }
        }

        /* free old cache and swap */
        free(cache);
        cache = new_cache;
        cache_size = n;
    }

    return cache[n-1];
}
