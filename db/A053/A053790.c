/*
 * A053790  Composite numbers arising as sum of first k primes.
 * https://oeis.org/A053790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth composite sum of the first k primes. The function
 *            generates the sums incrementally and caches them, so repeated
 *            calls are O(1)
 * Range:     u8 n <= 8, u16 n <= 137, u32 n <= 359, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big, next_prime, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

#ifdef __cplusplus
#include "../utils/seq_alloc_new_array.h"
#endif

seq_t a(idx_t n)
{
    /* Static state: caching computed terms and maintaining the current
       running sum of primes. */
    static seq_t *cache      = NULL; /* cached composite sums   */
    static size_t cache_cap  = 0;    /* allocated size of cache */
    static idx_t  cached_n      = 0; /* number of cached terms   */
    static seq_t   current_sum   = 0; /* running sum of primes    */
    static uint64_t current_prime = 1;/* last prime used          */
    static bool initialized = false; /* initialization flag     */

    /* One‑time allocation of cache storage. */
    if (!initialized)
    {
        initialized = true;
        cache_cap = 16; /* initial modest capacity */

#ifdef __cplusplus
        cache = seq_alloc_new_array(cache_cap);
#else
        /* In a purely C build we must use malloc; cast to seq_t* is fine. */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache)
            abort(); /* ensure that we don't continue with a null pointer */
    }

    /* Return immediately if the requested term has already been cached. */
    if (n <= cached_n)
        return cache[n - 1];

    /* Generate additional composite sums until we reach the desired index. */
    while (cached_n < n)
    {
        /* Get the next prime after the current one. */
        current_prime = next_prime(current_prime);

        /* Update the running sum; conversion to seq_t is safe for all
           supported seq_t sizes. */
        current_sum += (seq_t)current_prime;

        /* If the sum is composite, add it to the cache. */
        if (!is_prime_big(current_sum))
        {
            ++cached_n; /* new term discovered */

            /* Grow the cache if necessary. */
            if (cached_n > cache_cap)
                grow_seq(&cache, &cache_cap, (size_t)cached_n);

            cache[cached_n - 1] = current_sum;
        }
    }

    return cache[n - 1];
}
