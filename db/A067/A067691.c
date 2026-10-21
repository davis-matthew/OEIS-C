/*
 * A067691  a(n) = (a(n-1) + 1)^(n-1) for n > 0, a(0) = 0.
 * https://oeis.org/A067691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+1)^(n-1) for n>0, a(0)=0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp) */
#include "../utils/ensure_cache.h" /* ensure_cache(idx_t n, seq_t **cache, size_t *cap) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Ensure the cache can hold index n   */
    ensure_cache(n, &cache, &cap);

    /* Base case */
    if (n == 0)
    {
        cache[0] = 0;
        return 0;
    }

    /* If already computed, return it */
    if (cache[n] != 0)
        return cache[n];

    /* Compute all terms up to n, using the recurrence */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] == 0)   /* not yet computed */
        {
            seq_t prev   = cache[i - 1];
            seq_t base   = prev + 1;
            seq_t exp    = (seq_t)(i - 1);
            cache[i]     = big_pow(base, exp);
        }
    }

    return cache[n];
}
