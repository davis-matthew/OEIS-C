/*
 * A021981  Decimal expansion of 1/977.
 * https://oeis.org/A021981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/977) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth digit (0‑based) of the decimal expansion of 1/977 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache to keep digits already computed */
    static seq_t *cache = NULL;   /* array of digits 0‑9 */
    static idx_t  cache_len = 0;  /* number of digits stored (cache_len-1 is last index) */
    static idx_t  cache_cap = 0;  /* allocated capacity */
    static idx_t  remainder = 1;  /* current remainder after last computed digit */

    /* Increase cache capacity if needed */
    if (n + 1 > cache_cap)
    {
        idx_t new_cap = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            return 0;                       /* allocation failed, fallback zero */
        if (cache != NULL)
        {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute digits up to index n if not already computed */
    if (n >= cache_len)
    {
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* Long division step: r *= 10, digit = r / 977, r = r % 977 */
            remainder *= 10ULL;
            seq_t digit = (seq_t)(remainder / 977ULL);
            remainder   = remainder % 977ULL;
            cache[i] = digit;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
