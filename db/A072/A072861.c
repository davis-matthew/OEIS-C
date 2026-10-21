/*
 * A072861  a(n) = sigma(n)^2.
 * https://oeis.org/A072861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(n)^2 (sum of divisors of n, squared)
 * Range:     u8 n <= 9, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *sigma_cache = NULL;
static seq_t *a_cache     = NULL;
static idx_t  cache_size  = 0;   /* largest n for which cache is valid */

/* Return sigma(n) * sigma(n), caching results for future calls. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n = 0, return 0 */

    /* Expand caches if needed. */
    if (n > cache_size)
    {
        /* Allocate new arrays of size n+1 (index 0 unused). */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_sigma = (seq_t *)malloc(new_size * sizeof(seq_t));
        seq_t *new_a     = (seq_t *)malloc(new_size * sizeof(seq_t));

        if (!new_sigma || !new_a) {
            /* Allocation failure – fallback to zero result. */
            if (new_sigma) free(new_sigma);
            if (new_a)     free(new_a);
            return 0;
        }

        /* Initialise new arrays to zero. */
        memset(new_sigma, 0, new_size * sizeof(seq_t));
        memset(new_a, 0, new_size * sizeof(seq_t));

        /* Compute sigma(k) for k = 1..n. */
        for (idx_t i = 1; i <= n; ++i)
        {
            for (idx_t j = i; j <= n; j += i)
                new_sigma[j] += (seq_t)i;
        }

        /* Compute a(k) = sigma(k)^2. */
        for (idx_t i = 1; i <= n; ++i)
            new_a[i] = new_sigma[i] * new_sigma[i];

        /* Replace old caches. */
        free(sigma_cache);
        free(a_cache);
        sigma_cache = new_sigma;
        a_cache     = new_a;
        cache_size  = n;
    }

    /* Return cached value. */
    return a_cache[n];
}
