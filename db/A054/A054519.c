/*
 * A054519  Number of increasing arithmetic progressions of nonnegative
 *          integers ending in n, including those of length 1 or 2.
 * https://oeis.org/A054519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = a(n-1) + A000005(n) with a(0) = 1 (i.e. a(n) = 1 +
 *            sum_{i=1..n} d(i), where d(i) counts divisors of i)
 * Range:     u8 n <= 59, u16 n <= 7247, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Return number of positive divisors of n (A000005).  */
static seq_t divisor_count(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            ++cnt;
            if (i != n / i)
                ++cnt;
        }
    }
    return cnt;
}

/* Cached sequence values.  */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;   /* number of entries currently cached */

/* Ensure seq_cache has values up to index n inclusive.  */
static void ensure_up_to(idx_t n)
{
    if (cache_size == 0)
    {
        /* First allocation.  */
        seq_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!seq_cache)
            return;
        seq_cache[0] = (seq_t)1;     /* a(0) */
        cache_size = 1;
        /* Compute values from 1 to n.  */
        for (idx_t i = 1; i <= n; ++i)
        {
            seq_t d = divisor_count(i);
            seq_cache[i] = seq_cache[i - 1] + d;
        }
        cache_size = n + 1;
    }
    else if (cache_size <= n)
    {
        idx_t old_size = cache_size;
        seq_cache = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
        if (!seq_cache)
            return;
        /* Compute new values from old_size to n.  */
        for (idx_t i = old_size; i <= n; ++i)
        {
            seq_t d = divisor_count(i);
            seq_cache[i] = seq_cache[i - 1] + d;
        }
        cache_size = n + 1;
    }
    /* else: already up to date.  */
}

/* External function required by the build.  */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return (cache_size > n) ? seq_cache[n] : (seq_t)0;
}
