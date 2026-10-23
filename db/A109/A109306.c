/*
 * A109306  Numbers k such that k^2 + (k-1)^2 and k^2 + (k+1)^2 are both
 *          primes.
 * https://oeis.org/A109306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer k such that both
 * k² + (k−1)² and k² + (k+1)² are prime. 
 * The first term is 2. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/push_seq.h"

static seq_t *cache = NULL;      /* dynamic array of seq_t terms */
static idx_t cache_len = 0;      /* number of terms stored */
static idx_t cache_cap = 0;      /* allocated capacity of cache */

/* Append a value to the cache using the helper push_seq. */
static void cache_push(seq_t val)
{
    push_seq(&cache, &cache_cap, &cache_len, val);
}

/* Return the n-th term of A109306.  (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset irrelevant for n < 1 */

    /* Ensure that the first term (k = 2) is present. */
    if (cache_len == 0)
        cache_push((seq_t)2);

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* Next candidate k starts after the last stored term */
        uint64_t k = (uint64_t)cache[cache_len - 1] + 1;

        for (;; ++k)
        {
            /* Compute val1 = k² + (k-1)²  = 2k² - 2k + 1
             * Compute val2 = k² + (k+1)²  = 2k² + 2k + 1
             * Use 128‑bit intermediate to avoid overflow.
             */
            unsigned __int128 sq = (unsigned __int128)k * k;
            uint64_t val1 = (uint64_t)(2 * sq - 2 * k + 1);
            uint64_t val2 = (uint64_t)(2 * sq + 2 * k + 1);

            if (is_prime_uint64(val1) && is_prime_uint64(val2))
            {
                cache_push((seq_t)k);
                break;   /* accept this k and continue */
            }
        }
    }

    return cache[n - 1];
}
