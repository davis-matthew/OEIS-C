/*
 * A143721  Aliquot sequence starting at 38.
 * https://oeis.org/A143721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=38, a(n+1)=σ(a(n))−a(n), i.e. the aliquot sequence starting
 *            at 38
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Count of proper divisors of a uint64_t n, returned as seq_t. */
static seq_t sum_proper_divisors(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t sum = (seq_t)1;                      /* 1 is always a proper divisor */
    idx_t limit = 0;
    /* Compute floor(sqrt(n)) without using floating point */
    while ((limit + 1) * (limit + 1) <= n) ++limit;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (n % i == 0)
        {
            idx_t j = n / i;
            sum += (seq_t)i;
            if (j != i) sum += (seq_t)j;
        }
    }
    return sum;
}

/* Static cache for memoised sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Aliquot sequence starting at 38. */
seq_t a(idx_t n)
{
    if (cache_len == 0)
    {
        ensure_cache_cap(1);
        cache[0] = (seq_t)38;
        cache_len = 1;
    }

    if (n >= cache_len)
    {
        ensure_cache_cap((size_t)n + 1);
        for (idx_t i = cache_len; i <= n; ++i)
        {
            seq_t prev = cache[i - 1];
            seq_t next;
            if (prev == (seq_t)0)
                next = (seq_t)0;
            else
                next = sum_proper_divisors((idx_t)prev);
            cache[i] = next;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
