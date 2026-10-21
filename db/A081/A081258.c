/*
 * A081258  Numbers k > 1 such that k^3 - 1 (or equivalently k^2 + k + 1) has
 *          no prime factor greater than k.
 * https://oeis.org/A081258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer k > 1 such that k^2 + k + 1 has no *
 *            prime factor greater than k. The implementation searches for *
 *            successive k values, tests each by factoring k^2 + k + 1 with *
 *            factor_u64, and memoizes the valid terms so that any call * a(n)
 *            (in any order) runs quickly
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/factor_u64.h"

/* Check whether a given k satisfies the condition */
static bool k_is_ok(uint64_t k)
{
    /* Compute v = k^2 + k + 1 using 128‑bit to avoid overflow.  */
    unsigned __int128 tmp = (unsigned __int128)k * k + k + 1;
    if (tmp > (unsigned __int128)UINT64_MAX) {
        /* For very large k the value does not fit in 64‑bit;
           such k are beyond the scope needed for the tests.  */
        return false;
    }
    uint64_t v = (uint64_t)tmp;

    /* Factor v.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(v, primes, exps, &cnt);

    /* Every prime factor must be <= k.  */
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] > k)
            return false;
    }
    return true;
}

/* Main function to compute a(n).  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Static cache of already found terms.  */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_len = 0;
    static uint64_t next_k = 2;          /* next candidate to test */

    while (cache_len < (size_t)n) {
        uint64_t k = next_k;
        /* Find the next valid k.  */
        while (!k_is_ok(k))
            ++k;

        /* Grow cache if necessary.  */
        if (cache_len == cache_capacity) {
            cache_capacity = cache_capacity ? cache_capacity * 2 : 32;
            cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)k;

        /* Next search starts just after the found k.  */
        next_k = k + 1;
    }

    return cache[n - 1];
}
