/*
 * A008784  Numbers k such that sqrt(-1) mod k exists; or, numbers that are
 *          primitively represented by x^2 + y^2.
 * https://oeis.org/A008784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _a008784_is_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A008784 – numbers whose prime divisors are all ≡1 (mod 4) and that contain
    at most one factor of 2 (i.e. the odd part is composed only of primes 1 mod 4).   */
/*  The function generates the sequence on demand and caches results for
    subsequent calls.  The algorithm simply tests each integer in turn.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_a008784_is_good.h"

/*  a(n) – nth element of A008784, 1‑indexed.  */
seq_t a(idx_t n)
{
    static seq_t *cache       = NULL;   /* cached terms */
    static idx_t cache_len    = 0;      /* number of cached terms */
    static idx_t cache_cap    = 0;      /* necessary capacity */
    static seq_t next_num      = 1;     /* next number to examine */

    if (n <= 0) return (seq_t)0;        /* undefined – safety */

    /* Ensure enough capacity for the cache.  */
    if (cache == NULL) {
        cache_cap = 128;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);            /* allocation failed */
    }
    while (cache_len < n) {

        /* Test current number.  */
        if (_a008784_is_good((uint64_t)next_num))
        {
            /* Grow cache if needed.  */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = next_num;
        }
        ++next_num;
    }
    return cache[n - 1];
}
