/*
 * A157358  Triple-safe primes p: p, (p-1)/2, (p-3)/4, and (p-7)/8 are all
 *          prime.
 * https://oeis.org/A157358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157358: primes p such that (p-1)/2, (p-3)/4, and (p-7)/8
   are all prime.  The function returns the nth term (a(1)=23).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* candidate starting at the first known triple‑safe prime */
    static uint64_t next_cand = 23;   /* p ≡ 7 (mod 8) */

    if (n == 0)          /* index 0 is outside OEIS offset; not used */
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_size < n)
    {
        uint64_t p = next_cand;

        /* primality of p and its derived values */
        if (is_prime(p))
        {
            uint64_t half   = (p - 1) / 2;
            uint64_t quart  = (p - 3) / 4;
            uint64_t eighth = (p - 7) / 8;

            if (is_prime(half) && is_prime(quart) && is_prime(eighth))
            {
                /* grow cache if necessary */
                if (cache_size >= cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 32 : cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) /* allocation failure: return 0 as sentinel */
                        return 0;
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)p;
            }
        }
        next_cand += 8;   /* stay on numbers ≡ 7 (mod 8) */
    }

    return cache[n - 1];
}
