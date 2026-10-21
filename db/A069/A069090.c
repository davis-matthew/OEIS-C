/*
 * A069090  Primes none of whose proper initial segments are primes.
 * https://oeis.org/A069090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   primes whose proper initial segments are not primes.
 *            Search-based implementation
 * Range:     u8 n <= 27, u16 n <= 1636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static idx_t current = 2;

    while (cache_sz < n)
    {
        /* Skip even numbers greater than 2. */
        if (current > 2 && !(current & 1))
        {
            current++;
            continue;
        }

        /* Test primality of the candidate. */
        if (isPrimeTrial((seq_t)current))
        {
            idx_t x = current;
            bool ok = true;
            while (x > 0)
            {
                x /= 10;
                if (x == 0) break;
                if (isPrimeTrial((seq_t)x))
                {
                    ok = false;
                    break;
                }
            }

            if (ok)
            {
                if (cache_sz == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_sz++] = (seq_t)current;
            }
        }
        current++;
    }

    return cache[n - 1];
}
