/*
 * A065341  Mersenne composites: 2^prime(m) - 1 is not a prime.
 * https://oeis.org/A065341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   is_prime_big, pow2, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065341: a(n) = 2^p - 1 where p is the n‑th prime that makes 2^p-1 composite. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of previously found Mersenne composites */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* next integer to test for primality (prime candidates) */
    static uint64_t next_candidate = 2;

    if (n == 0)          /* offset starts at 1, n = 0 is unused */
        return (seq_t)0;

    /* fill cache until it holds at least n terms */
    while (cache_len < n)
    {
        /* find next prime number p */
        uint64_t p;
        while (true)
        {
            if (is_prime_big((seq_t)next_candidate))
            {
                p = next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }

        /* compute 2^p - 1 */
        seq_t mersenne = pow2((idx_t)p);
        mersenne -= (seq_t)1;            /* subtract 1 */

        /* keep if it is composite */
        if (!is_prime_big(mersenne))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                if (cache == NULL) abort();   /* allocation failure */
                cache_cap = new_cap;
            }
            cache[cache_len++] = mersenne;
        }
    }

    return cache[n - 1];
}
