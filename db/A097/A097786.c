/*
 * A097786  a(n) = 3*a(n-1) + C(n+3,3) for n > 0; a(0)=1.
 * https://oeis.org/A097786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 3*a(n-1) + C(n+3,3)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, abort */

static seq_t *cache = NULL;   /* cached values */
static idx_t cache_len = 0;   /* number of elements cached */

/* Ensure that a value for every index up to n is computed. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)   /* already have what we need */
        return;

    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL)
        abort();               /* allocation failed */

    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)        /* base case */
        {
            cache[0] = 1;
            continue;
        }

        /* C(i+3,3) = (i+3)(i+2)(i+1)/6 */
        seq_t tri = ((seq_t)(i + 3) * (seq_t)(i + 2) * (seq_t)(i + 1)) / 6;

        /* recurrence */
        cache[i] = 3 * cache[i - 1] + tri;
    }

    cache_len = new_len;
}

/* Public entry point required by the framework */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
