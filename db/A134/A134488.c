/*
 * A134488  a(0)=1. a(n) = n + d(a(n-1)), where d(m) is the number of positive
 *          divisors of m.
 * https://oeis.org/A134488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=n + d(a(n-1))   (n>=1)
   where d(m) is the number of positive divisors of m. */

#include <stdlib.h>
#include <stddef.h>

/* Count divisors of m for any unsigned integer type seq_t.
   Uses trial division up to sqrt(m).  Works for seq_t that
   support +, -, *, /, % and overflow is impossible for p≤sqrt(m). */
static seq_t divisor_count(seq_t m)
{
    if (m <= 1)
        return 1;
    seq_t count = 1;
    seq_t p = 2;
    /* loop while p ≤ m/p to avoid computing sqrt(m). */
    while (p <= m / p)
    {
        if (m % p == 0)
        {
            seq_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
        ++p;
    }
    if (m > 1)            /* remaining prime factor */
        count *= 2;
    return count;
}

/* Compute a(n) with memoization.  The cache stores a(0)…a(len‑1).
   On first call it creates the entry for a(0).  Subsequent calls
   extend the cache as needed. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache      = NULL;   /* pointer to sequence values   */
    static size_t cache_cap  = 0;      /* allocated capacity            */
    static size_t cache_len  = 0;      /* number of values computed     */

    /* initialise cache */
    if (cache_cap == 0)
    {
        cache_cap = 1;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0]  = 1;      /* a(0)=1 */
        cache_len = 1;
    }

    /* extend capacity if necessary (doubling strategy) */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                /* double */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t d    = divisor_count(prev);
        cache[(size_t)i] = (seq_t)i + d;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
