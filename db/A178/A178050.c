/*
 * A178050  n=x^2+17, n and n+2 are prime.
 * https://oeis.org/A178050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: numbers n = x^2 + 17 such that n and n+2 are prime (twin primes). */
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_realloc.h"   /* provides seq_realloc(seq_t*, size_t, size_t) */
#include "../utils/is_prime_big.h" /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* static cache for previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t count    = 0;   /* number of terms stored in cache */
    static idx_t next_x   = 0;   /* next x to test (x >= 0) */

    if (n == 0)   /* offset starts at 1, behaviour for n==0 is undefined */
        return 0;

    /* if the requested term is already cached, return it immediately */
    if (n <= count)
        return cache[n - 1];

    /* generate terms until we reach the nth one */
    for (;;)
    {
        seq_t candidate = (seq_t)next_x * (seq_t)next_x + (seq_t)17;

        /* check whether candidate and candidate+2 are prime */
        if (is_prime_big(candidate) && is_prime_big(candidate + (seq_t)2))
        {
            /* enlarge cache if necessary */
            if (count == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp     = seq_realloc(cache, cache_cap, new_cap);
                cache          = tmp;
                cache_cap      = new_cap;
            }
            cache[count] = candidate;
            ++count;
        }
        ++next_x;
        if (count >= n)
            break;
    }
    return cache[n - 1];
}
