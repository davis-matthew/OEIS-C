/*
 * A089440  14*p+13 and p are primes.
 * https://oeis.org/A089440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th prime p for which 14*p+13 is also prime (offset
 *            1). The function generates candidates in order and memoises the
 *            results in a static cache. It uses the helper bool
 *            is_prime(idx_t x) from ../utils/is_prime.h
 * Range:     u8 n <= 4, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 979 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t cur = 2;            /* next candidate prime */
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
        initialized = true;
    }

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n) {
        /* Find the next qualifying prime */
        while (true) {
            if (is_prime(cur)) {
                /* Compute 14*p + 13 using seq_t arithmetic */
                seq_t q = ((seq_t)cur) * 14 + 13;
                if (is_prime((idx_t)q))
                    break;       /* cur is a valid term */
            }
            cur++;               /* try next candidate */
        }

        /* Store the found prime */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len++] = (seq_t)cur;
        cur++;                   /* advance past the stored prime for next search */
    }

    return cache[n - 1];
}
