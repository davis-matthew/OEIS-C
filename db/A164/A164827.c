/*
 * A164827  Generalized Lucas numbers: a(n) = a(n-1) + 10*a(n-2); with a(1)=2
 *          a(2)=1.
 * https://oeis.org/A164827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-2); with a(1)=2, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <assert.h>

/* Enough capacity for the typical test range (≈10000). */
#define CACHE_CAP 20000

static seq_t cache[CACHE_CAP + 1];
static idx_t last_computed = 0;   /* highest index whose value is cached */

/* Initialise the memoisation table with the two seed values. */
static void init_cache(void)
{
    if (last_computed >= 2)
        return;
    cache[0] = (seq_t)0;      /* unused, keeps indexes 1‑based */
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)1;
    last_computed = 2;
}

seq_t a(idx_t n)
{
    assert(n >= 1);           /* sequence is defined only for n ≥ 1 */
    init_cache();

    /* If n lies within the cached window: extend the cache if needed. */
    if (n <= CACHE_CAP)
    {
        if (n > last_computed)
        {
            for (idx_t i = last_computed + 1; i <= n; ++i)
            {
                cache[i] = cache[i - 1] + (seq_t)10 * cache[i - 2];
            }
            last_computed = n;
        }
        return cache[n];
    }

    /* For n larger than the cached window, compute on the fly. */
    seq_t prev1 = cache[2];   /* a(2) */
    seq_t prev2 = cache[1];   /* a(1) */
    if (n == 1)
        return cache[1];
    if (n == 2)
        return cache[2];

    seq_t val;
    for (idx_t i = 3; i <= n; ++i)
    {
        val = prev1 + (seq_t)10 * prev2;
        prev2 = prev1;
        prev1 = val;
    }
    return val;
}
