/*
 * A101789  Safe primes of the form 8*k-1: primes of the form 8*k-1 such that
 *          4*k-1 is also a prime.
 * https://oeis.org/A101789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*k-1 where k is the nth integer such that both 4*k-1 and
 *            8*k-1 are prime. The function keeps a static cache of found
 *            terms and extends it on demand
 * Range:     u8 n <= 4, u16 n <= 234, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

#define SAFE_PRIME_CACHE_CAP 20000   /* large enough for n ≤ 10000 */

seq_t a(idx_t n)
{
    static seq_t cache[SAFE_PRIME_CACHE_CAP];
    static idx_t cache_size = 0;
    static idx_t last_k = 0;

    /* Extend cache until it contains at least n terms. */
    while (cache_size < n)
    {
        ++last_k;
        uint64_t p2 = 4ULL * last_k - 1ULL;   /* 4k – 1  */
        uint64_t p1 = 8ULL * last_k - 1ULL;   /* 8k – 1  */

        if (is_prime(p2) && is_prime(p1))
        {
            if (cache_size >= SAFE_PRIME_CACHE_CAP)
                abort();                      /* should not happen for n ≤ 10000 */

            cache[cache_size++] = (seq_t)p1;
        }
    }
    return cache[n - 1];
}
