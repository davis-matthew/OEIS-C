/*
 * A164539  a(n) = 2*a(n-1) + 7*a(n-2) for n > 1; a(0) = 1, a(1) = 13.
 * https://oeis.org/A164539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 13, a(n) = 2*a(n-1) + 7*a(n-2) (n > 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Static cache – grows only when necessary */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of allocated entries */

    if (n >= cache_len)
    {
        /* Allocate new array of required size */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = seq_alloc_array(new_len);
        if (!tmp)   /* allocation failure – unlikely in tests */
            return (seq_t)0;

        /* Copy existing values to the new array */
        for (idx_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];

        /* Free old storage if it existed */
        if (cache)
            seq_free_array(cache);

        /* Compute missing terms starting from cache_len */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
                tmp[0] = (seq_t)1;
            else if (i == 1)
                tmp[1] = (seq_t)13;
            else
                tmp[i] = tmp[i-1] * (seq_t)2 + tmp[i-2] * (seq_t)7;
        }

        cache = tmp;
        cache_len = new_len;
    }

    return cache[n];
}
