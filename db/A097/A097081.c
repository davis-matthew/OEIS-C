/*
 * A097081  a(n) = Sum_{k=0..n} C(n,4k)*2^k.
 * https://oeis.org/A097081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} binomial(n,4k) * 2^k Recurrence (n ≥ 4):
 *            a(0) = a(1) = a(2) = a(3) = 1 a(n) = 4*a(n-1) - 6*a(n-2) +
 *            4*a(n-3) + a(n-4) This implementation uses memoised recurrence
 *            for efficiency
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;          /* cached values */
static idx_t  cache_len = 0;          /* number of stored entries */

/* Initialise cache with a(0) … a(3) = 1 */
static void
init_cache(void)
{
    if (cache != NULL)
        return;
    cache = seq_alloc_new_array(4);
    if (cache == NULL)
        exit(EXIT_FAILURE);
    for (idx_t i = 0; i < 4; ++i)
        cache[i] = (seq_t)1;
    cache_len = 4;
}

/* Expand the cache to hold values up to n (inclusive) */
static void
extend_cache(idx_t n)
{
    if (cache_len > n)
        return;
    idx_t new_len = n + 1;
    seq_t *tmp = seq_alloc_new_array(new_len);
    if (tmp == NULL)
        exit(EXIT_FAILURE);
    /* Copy old entries */
    for (idx_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    /* Free old buffer */
    free_seq_array(cache);
    cache = tmp;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len)
        return cache[n];

    idx_t old_len = cache_len;
    extend_cache(n);

    for (idx_t i = old_len; i <= n; ++i)
    {
        seq_t t = 4 * cache[i - 1];
        t -= 6 * cache[i - 2];
        t += 4 * cache[i - 3];
        t += cache[i - 4];
        cache[i] = t;
    }

    return cache[n];
}
