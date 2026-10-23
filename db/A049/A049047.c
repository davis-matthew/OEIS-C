/*
 * A049047  Number of distinct factorial numbers congruent to 1 (mod
 *          prime(n)).
 * https://oeis.org/A049047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m (1 ≤ m < p_n) such that m! ≡ 1 (mod p_n),
 *            where p_n is the nth prime. a(1) = 1. Indexing starts at 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;        /* cached values of a(k), 0‑based */
static idx_t cache_len = 0;          /* number of values currently cached */

seq_t a(idx_t n)
{
    if (n < 1)                          /* undefined for n < 1 */
        return 0;

    /* Grow the cache to hold values up to n, computing any missing ones */
    if (n > cache_len)
    {
        size_t need = (size_t)n;
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!cache)                     /* out of memory: best we can do */
            return 0;

        for (idx_t i = cache_len; i < n; ++i) /* i is 0‑based index */
        {
            uint32_t p = (uint32_t)get_prime(i + 1);  /* p_n, 1‑based */
            seq_t  cnt = 0;
            uint32_t fact = 1;                      /* factorial modulo p */

            for (uint32_t k = 1; k < p; ++k)
            {
                fact = (uint32_t)(( (uint64_t)fact * k ) % p);
                if (fact == 1)
                    ++cnt;
            }
            cache[i] = cnt;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
