/*
 * A089079  Numbers n such that 7*n - 23 is prime.
 * https://oeis.org/A089079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A089079: Numbers n such that 7*n-23 is prime.
 *  a(1)=4, a(2)=6, a(3)=10, …
 *
 *  The function caches values in a static array, generating new terms
 *  on demand.  is_prime_big() is used to test primality of 7*n-23.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Return the n-th term of the sequence.
     * n is 1‑based (offset=1).  For n<1 the function returns 0.  */
    if (n < 1)
        return 0;

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len   = 0;   /* number of terms stored */
    static idx_t cache_cap   = 0;   /* capacity of cache */
    static idx_t next_candidate = 1;/* next n to test */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < n)
    {
        seq_t p = (seq_t)7 * (seq_t)next_candidate - (seq_t)23;
        if (is_prime_big(p))
        {
            /* Ensure there is room for one more element */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
