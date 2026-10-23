/*
 * A179121  Variant of Sylvester's sequence: a(n + 1) = a(n)^2 - a(n) + 1,
 *          with a(1) = 18.
 * https://oeis.org/A179121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 18; a(n+1) = a(n)^2 - a(n) + 1 (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t len = 0;          /* number of terms stored */
    static idx_t cap = 0;          /* current capacity of cache */

    /* Grow cache to hold up to n terms */
    if (n > len)
    {
        /* Ensure sufficient capacity */
        if (cap < n)
        {
            idx_t new_cap = (cap == 0) ? n : (cap << 1);
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        /* Initialize first term if cache is empty */
        if (len == 0)
        {
            cache[0] = (seq_t)18;
            len = 1;
        }

        /* Compute missing terms up to n */
        for (idx_t i = len + 1; i <= n; ++i)
        {
            seq_t prev = cache[i - 2];
            cache[i - 1] = prev * prev - prev + (seq_t)1;
        }
        len = n;
    }

    return cache[n - 1];
}
