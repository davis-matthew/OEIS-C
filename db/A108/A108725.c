/*
 * A108725  Numbers n such that 11*n + 19 is prime.
 * https://oeis.org/A108725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 5838, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for n such that 11*n + 19 is prime.
   The function keeps a cache of found terms and extends it as
   necessary for each call. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit values. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, return 0 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocation capacity */
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t checked = 0;        /* next candidate to test */

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        unsigned __int128 val128 = (unsigned __int128)checked * 11 + 19;
        if (val128 <= (unsigned __int128)UINT64_MAX) {
            uint64_t val = (uint64_t)val128;
            if (is_prime_uint64(val)) {
                /* store the candidate in the cache */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);   /* memory allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)checked;
            }
        } else {
            /* value too large for 64‑bit primality test; stop searching */
            break;
        }
        checked++;
    }

    if (n <= cache_len)
        return cache[n - 1];

    /* fallback – should not occur for requested n within limits */
    return 0;
}
