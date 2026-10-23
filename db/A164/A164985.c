/*
 * A164985  Denominators of ternary BBP-type series for log(5).
 * https://oeis.org/A164985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 162*a(n-3) - 6561*a(n-6) for n>=6, with a(0)=1, a(1)=6,
 *            a(2)=27, a(3)=405, a(4)=1458, a(5)=5103
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static const seq_t init_vals[6] = { (seq_t)1, (seq_t)6, (seq_t)27,
                                        (seq_t)405, (seq_t)1458, (seq_t)5103 };

    /* Grow cache if needed */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0; /* allocation failure, return 0 */

        /* Copy existing terms to new cache */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Initialize newly added terms */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            if (i < 6)
                new_cache[i] = init_vals[i];
            else
                new_cache[i] = (seq_t)162 * new_cache[i - 3]
                               - (seq_t)6561 * new_cache[i - 6];
        }

        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
