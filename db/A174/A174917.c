/*
 * A174917  Lesser of twin primes p1 such that p2+(p2^2-p1^2) is a prime
 *          number.
 * https://oeis.org/A174917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 230, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the lesser member of a twin prime pair (p,p+2) such that 5p+6 is prime. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* cache of results */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* last prime examined during search */
    static seq_t last_checked = 1;

    while (cache_len < (size_t)n)
    {
        /* Find the next prime greater than last_checked */
        seq_t cand = last_checked + 1;
        if (cand <= 2)
            cand = 3;
        else if ((cand & 1) == 0)
            cand++;                 /* make odd */

        while (!is_prime_big(cand))
            cand += 2;              /* skip even numbers */

        /* cand is a prime. Test if it forms a twin with cand+2 */
        if (is_prime_big(cand + 2))
        {
            /* Check the additional condition 5*p + 6 is prime */
            seq_t val = cand * 5 + 6;
            if (is_prime_big(val))
            {
                /* Store the prime in the cache */
                if (cache_len == cache_cap)
                    grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = cand;
            }
        }

        /* Advance the search */
        last_checked = cand;
    }

    return cache[n - 1];
}
