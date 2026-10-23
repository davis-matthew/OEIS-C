/*
 * A109453  Cumulative sum of initial digits of n.
 * https://oeis.org/A109453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + first_digit(n) with a(0) = 0
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* first_digit(x) returns the most significant decimal digit of x */

#include <stdlib.h>

static inline seq_t first_digit(idx_t x)
{
    idx_t pow = 1;
    /* find 10^(k-1) where k is the number of decimal digits of x */
    while (x >= pow * 10)
        pow *= 10;
    return (seq_t)(x / pow);
}

/* Memoized cumulative sums.
   cache[0] = 0, cache[i] = a(i) for i ≥ 1. */
static seq_t *cache     = NULL;
static idx_t cache_size = 0;  /* highest index that has been computed (≥ 0) */
static idx_t cache_cap  = 0;  /* allocated buffer size */

seq_t a(idx_t n)
{
    if (n < 1u)            /* offset is 1; below return 0 */
        return (seq_t)0;

    if (cache_size < n) {
        /* need to grow the cache */
        idx_t new_cap = (cache_cap == 0 ? 16 : cache_cap);
        while (new_cap <= n)
            new_cap <<= 1;

        /* reallocate memory; cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed – abort gracefully */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;

        if (cache_size == 0) {
            /* first element is a(0) = 0 */
            cache[0] = (seq_t)0;
        }

        /* compute values up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + first_digit(i);

        cache_size = n;
    }

    return cache[n];
}
