/*
 * A064609  Partial sums of A034448: sum of unitary divisors from 1 to n.
 * https://oeis.org/A064609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{j=1..n} σ_u(j) where σ_u(j) is the sum of unitary
 *            divisors of j
 * Range:     u8 n <= 19, u16 n <= 309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Utility: integer square root for uint64_t */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* Compute the sum of unitary divisors of m. */
static seq_t unitary_sum(idx_t m)
{
    seq_t result = (seq_t)1;
    idx_t x = m;
    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            /* compute p^e */
            idx_t pw = 1;
            while (x % p == 0)
            {
                pw *= p;
                x /= p;
            }
            result *= ((seq_t)1 + (seq_t)pw);
        }
    }
    if (x > 1) /* leftover prime factor */
    {
        result *= ((seq_t)1 + (seq_t)x);
    }
    return result;
}

/* Dynamic cache for computed partial sums */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;
static seq_t current_sum = 0;

/* Ensure the cache can hold at least n+1 elements (indexing from 0). */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;
    size_t new_cap = (n + 1) * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined offset, but safe */

    ensure_cache(n);
    if (n > max_computed)
    {
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            seq_t usum = unitary_sum(i);
            current_sum += usum;
            cache[i] = current_sum;
        }
        max_computed = n;
    }
    return cache[n];
}
