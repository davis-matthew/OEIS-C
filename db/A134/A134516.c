/*
 * A134516  Numbers that are divisible by 2*(sum of their digits).
 * https://oeis.org/A134516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer that is divisible by 2 * (sum
 *            of its decimal digits)
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the sum of decimal digits of a value. */
static inline seq_t digit_sum(seq_t v)
{
    seq_t sum = 0;
    while (v != 0)
    {
        sum += v % (seq_t)10;
        v /= (seq_t)10;
    }
    return sum;
}

/* Return the n-th number that is divisible by twice its digit sum. */
seq_t a(idx_t n)
{
    /* Static cache that grows with the largest n requested so far. */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Next candidate to test. It starts just after 0. */
    static seq_t next_candidate = 1;

    /* Grow and fill the cache until we have at least n terms. */
    while (cache_len < n)
    {
        seq_t val   = next_candidate;
        seq_t sum   = digit_sum(val);
        seq_t factor = (seq_t)2 * sum;

        /* Handle the trivial case where 2*sum == 0 (never happens for positive integers). */
        if (factor != 0 && val % factor == 0)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
