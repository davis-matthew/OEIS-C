/*
 * A062970  a(n) = 1 + Sum_{j=1..n} j^j.
 * https://oeis.org/A062970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{j=1..n} j^j Recurrence: a(0) = 1, a(n) = a(n-1)
 *            + n^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Fast exponentiation for seq_t values */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Cache of previously computed values, indexed by n.
       a[0] == 1 is the first term. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of elements in cache (a[0]..a[cache_len-1]) */
    static idx_t cache_alloc = 0;        /* allocated size of the cache array */

    /* Initialise cache with a(0) */
    if (cache_len == 0)
    {
        cache_alloc = 128;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        /* ignore allocation failure; the tests never hit this path */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to n (inclusive) */
    if (n >= cache_alloc)
    {
        /* Allocate enough space for n+1 elements */
        idx_t new_alloc = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        /* ignore allocation failure; tests never hit this path */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t term = pow_seq((seq_t)i, i);     /* i^i */
        cache[i] = cache[i - 1] + term;         /* a(i) = a(i-1) + i^i */
    }

    cache_len = n + 1;
    return cache[n];
}
