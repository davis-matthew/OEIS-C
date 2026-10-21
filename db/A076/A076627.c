/*
 * A076627  a(n) = tau(n)*(n-tau(n)), where tau(n) = number of divisors of n
 *          (A000005).
 * https://oeis.org/A076627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) * (n - tau(n)), where tau(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 39, u16 n <= 1679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/tau_of_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Static cache: cache[n] holds a(n) for 1 ≤ n ≤ cache_size. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n > cache_size)
    {
        /* Allocate larger cache.  The array is 1‑based; index 0 is unused. */
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)malloc((new_size + 1) * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure */

        /* Preserve existing values. */
        for (idx_t i = 1; i <= cache_size; ++i)
            tmp[i] = cache[i];

        /* Compute and store new terms. */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t tau = tau_of_uint64((uint64_t)i);
            tmp[i] = tau * (((seq_t)i) - tau);
        }

        /* Replace old cache. */
        free(cache);
        cache = tmp;
        cache_size = n;
    }

    return cache[n];
}
