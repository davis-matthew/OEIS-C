/*
 * A106909  Primes of the form 4x^2-3xy+4y^2, with x and y nonnegative.
 * https://oeis.org/A106909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that p = 4x^2-3xy+4y^2 with x,y≥0. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Compute floor(sqrt(x)) for 0 <= x <= 2^128-1. */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0;
    uint64_t hi = 1;
    while ((unsigned __int128)hi * hi <= x)
        hi <<= 1;                       /* find hi > sqrt(x)      */
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Return true iff a prime p can be written as 4x^2-3xy+4y^2 with
   x,y >= 0. */
static inline bool is_repr_prime(uint64_t p)
{
    unsigned __int128 left = (unsigned __int128)16 * p;  /* 16p - 55y^2 = k^2 */
    for (uint64_t y = 0; ; ++y) {
        unsigned __int128 term = (unsigned __int128)55 * y * y;
        if (term > left)
            break;
        unsigned __int128 diff = left - term;          /* k^2 */
        uint64_t k = isqrt_uint128(diff);
        if ((unsigned __int128)k * k != diff)
            continue;
        uint64_t t1 = 3 * y + k;
        if (t1 % 8 == 0)
            return true;
        if (3 * y >= k) { /* avoid underflow for t2 */
            uint64_t t2 = 3 * y - k;
            if (t2 % 8 == 0)
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n < 1 */

    /* static cache of the sequence */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;
    static uint64_t last_checked = 1;   /* last integer tested for primality */

    while (cache_size < n) {
        ++last_checked;
        if (!is_prime(last_checked))
            continue;                  /* skip composite numbers */
        if (!is_repr_prime(last_checked))
            continue;                  /* not of the required form */

        /* add prime to cache */
        if (cache_size >= cache_cap) {
            size_t newcap = (cache_cap == 0) ? 8 : (size_t)cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(1);        /* allocation failed */
            cache = tmp;
            cache_cap = (idx_t)newcap;
        }
        cache[cache_size++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}
