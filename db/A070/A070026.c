/*
 * A070026  Initial, all intermediate and final iterated sums of digits of n
 *          are primes.
 * https://oeis.org/A070026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   sequence of natural numbers whose all iterated digit sums are
 *            prime; computed by searching from 2 upwards
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Sum of the decimal digits of x. */
static inline seq_t digit_sum_seq(seq_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Test whether all iterated digit sums of x are prime. */
static inline bool qualifies(seq_t x)
{
    seq_t s = digit_sum_seq(x);
    if (!is_prime_big(s))
        return false;
    while (s > (seq_t)9)
    {
        s = digit_sum_seq(s);
        if (!is_prime_big(s))
            return false;
    }
    return true;
}

/* Return the nth term of A070026. Indexing starts at 1. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 2;   /* start from the first positive integer */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n)
    {
        /* Find the next qualifying integer. */
        while (!qualifies(next_candidate))
            ++next_candidate;

        /* Expand the cache if needed. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the new term and advance the candidate. */
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
