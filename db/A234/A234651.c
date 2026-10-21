/*
 * A234651  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234651
   Recurrence: a(n) = 4*a(n-1) + 44*a(n-2) - 176*a(n-3) - 480*a(n-4) + 1920*a(n-5)
   Initial values: a(1)=160, a(2)=728, a(3)=3264, a(4)=15008, a(5)=67840
*/
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;      /* 1‑based cache of computed terms   */
static size_t cache_size = 0;    /* current allocated size            */
static idx_t max_computed = 0;   /* largest n already filled          */

/* enlarge the cache to at least n+1 elements, zero‑initialising new slots */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)(n + 1) > cache_size)
    {
        size_t old = cache_size;
        ensure_capacity(n + 1, &cache_size, (void **)&cache, sizeof(seq_t));
        for (size_t i = old; i < cache_size; ++i)
            cache[i] = (seq_t)0;
    }
}

/* initialise base values on first use */
static void init_cache(void)
{
    ensure_cache_size(5);
    cache[1] = (seq_t)160;
    cache[2] = (seq_t)728;
    cache[3] = (seq_t)3264;
    cache[4] = (seq_t)15008;
    cache[5] = (seq_t)67840;
    max_computed = 5;
}

seq_t a(idx_t n)
{
    /* n is >=1 by sequencing convention; behaviour for n<1 is undefined   */
    if (!cache)
        init_cache();

    if (n <= max_computed)
        return cache[n];

    ensure_cache_size(n);

    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t value = cache[i - 1] * (seq_t)4
                    + cache[i - 2] * (seq_t)44
                    + cache[i - 5] * (seq_t)1920;
        value -= cache[i - 3] * (seq_t)176;
        value -= cache[i - 4] * (seq_t)480;
        cache[i] = value;
    }

    max_computed = n;
    return cache[n];
}
