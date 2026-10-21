/*
 * A200088  a(n) is the least number greater than a(n-1) such that the sum of
 *          the decimal digits of a(n-1) and a(n) is prime; starting with
 *          a(1)=1.
 * https://oeis.org/A200088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) = least m > a(n-1) such that
   digit_sum(a(n-1)) + digit_sum(m) is prime. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_sum.h"

/* Small primality test for operands that fit in an int */
static inline bool is_prime_small(int n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (int d = 5; d * d <= n; d += 6)
        if (n % d == 0 || n % (d + 2) == 0)
            return false;
    return true;
}

/* static cache for memoisation */
static seq_t *cache      = NULL;   /* indices 0..cache_len-1 store a(1)..a(cache_len) */
static size_t cache_len  = 0;     /* number of terms cached */
static size_t cache_cap  = 0;     /* current capacity of cache */

/* Ensure the cache can accommodate 'needed' terms */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined index: return 0 */

    /* Fast return if value already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Grow cache up to requested index */
    for (size_t idx = cache_len; idx < (size_t)n; ++idx)
    {
        if (idx == 0)                /* a(1) = 1 */
        {
            ensure_cache_capacity(1);
            cache[0] = (seq_t)1;
            cache_len = 1;
            continue;
        }

        seq_t prev  = cache[idx - 1];
        seq_t cand  = prev + 1;
        int   sum_prev = (int)digit_sum(prev);

        while (true)
        {
            int sum_cand = (int)digit_sum(cand);
            if (is_prime_small(sum_prev + sum_cand))
                break;
            ++cand;
        }

        ensure_cache_capacity(idx + 1);
        cache[idx] = cand;
    }

    return cache[(size_t)n - 1];
}
