/*
 * A072489  a(1) = 1, a(n) = a(n-1) times smallest divisor of n >= n^(1/2).
 * https://oeis.org/A072489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=a(n-1)*A033677(n) for n>1 where A033677(n) is the
 *            smallest divisor of n that is >= sqrt(n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static inline idx_t floor_isqrt_u64(idx_t n)
{
    /* Binary search for floor(sqrt(n)) with 64‑bit arithmetic. */
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;       /* 2^32 > sqrt(2^64-1) */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Smallest divisor of x that is ≥ sqrt(x).  Assumes x ≥ 2. */
static inline seq_t smallest_divisor_ge_sqrt(idx_t x)
{
    idx_t root = floor_isqrt_u64(x);
    for (idx_t d = root; d > 0; --d)
    {
        if (x % d == 0)
            return (seq_t)(x / d);   /* complement, ≥ sqrt(x) */
    }
    /* Fallback (should not happen for x ≥ 2). */
    return (seq_t)x;
}

/* Memoised computation of a(n).  Cache holds a(i) for i < cache_len. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n=0, return 0 */

    /* Initialise cache for a(1). */
    if (cache_len == 0)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;     /* a(1) = 1 */
        cache_len = 1;
    }

    /* Extend cache up to requested n. */
    if (n > (idx_t)cache_len)
    {
        idx_t new_len = n;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            seq_t d = smallest_divisor_ge_sqrt(i);
            cache[i - 1] = cache[i - 2] * d;
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
