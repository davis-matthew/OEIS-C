/*
 * A163870  Triangle read by rows: row n lists the nontrivial divisors of the
 *          n-th composite.
 * https://oeis.org/A163870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2256, u16 n <= 6281, u32 n <= 6281, u64 n <= 6281,
 *            u128 n <= 6281, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates sequence A163870: a(n) is the nth non‑trivial divisor
   of the nth composite number, read row‑wise. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* handle the (unlikely) case of a zero index */
    if (n == 0) return (seq_t)0;

    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_composite = 4;   /* first composite number */

    /* grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* skip over primes to find the next composite */
        while (is_prime(next_composite))
            ++next_composite;

        idx_t c = next_composite;

        /* arrays holding the small and large divisors of c   */
        idx_t small[512];
        idx_t large[512];
        size_t small_cnt = 0;
        size_t large_cnt = 0;

        /* enumerate divisors from 2 to sqrt(c) */
        for (idx_t d = 2; d <= c / d; ++d)
        {
            if (c % d == 0)
            {
                small[small_cnt++] = d;
                idx_t other = c / d;
                if (other != d)                /* avoid duplicate for squares */
                    large[large_cnt++] = other;
            }
        }

        /* ensure capacity for new terms */
        size_t needed = cache_len + small_cnt + large_cnt;
        if (needed > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* append the divisors in ascending order */
        for (size_t i = 0; i < small_cnt; ++i)
            cache[cache_len++] = (seq_t)small[i];

        for (size_t i = large_cnt; i > 0; --i)
            cache[cache_len++] = (seq_t)large[i - 1];

        /* move to the next integer for the next call */
        ++next_composite;
    }

    return cache[(size_t)n - 1];
}
