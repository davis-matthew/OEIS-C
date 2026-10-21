/*
 * A201119  Primes of the form 4k^3+5.
 * https://oeis.org/A201119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 4k^3+5 (k ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 159, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache for already found terms */
    static seq_t *cache      = NULL;   /* array of found primes */
    static size_t cache_len  = 0;      /* number of primes stored */
    static size_t cache_cap  = 0;      /* allocated capacity of cache */

    /* The next value of k to test for primality */
    static idx_t next_k = 0;

    /* If callers give n ≤ 0, behaviour is undefined; ignore such cases. */
    if (n == 0) return 0;

    /* Generate primes until we have at least n terms stored. */
    while (cache_len < n)
    {
        /* Search for the next prime of the form 4k^3+5. */
        while (true)
        {
            seq_t k   = (seq_t)next_k;
            seq_t k2  = k * k;
            seq_t k3  = k2 * k;
            seq_t val = k3 * 4 + 5;  /* 4*k^3 + 5 */

            if (isPrimeGeneric(val))
            {
                /* Ensure cache capacity. */
                if (cache_len == cache_cap)
                    grow_seq(&cache, &cache_cap, cache_cap ? cache_cap * 2 : 4);

                cache[cache_len++] = val;
                ++next_k;          /* move to next k for the next prime */
                break;             /* found a prime, go back to outer loop */
            }
            ++next_k;              /* test the next k */
        }
    }

    return cache[n - 1];
}
