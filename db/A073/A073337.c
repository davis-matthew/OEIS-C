/*
 * A073337  Primes of the form 4*k^2 - 10*k + 7 with k positive.
 * https://oeis.org/A073337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   primes of the form 4*k^2 - 10*k + 7 for k > 0 (offset = 1)
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 3660, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache for primes found */
    static seq_t   *cache   = NULL;
    static size_t   cache_len = 0;
    static size_t   cache_cap = 0;
    static idx_t    next_k   = 2;          /* start checking k = 2 */

    /* behavior for n < 1 is undefined; return 0 for safety */
    if (n == 0) return 0;

    /* return already computed term */
    if (cache_len >= n)
        return cache[(size_t)(n - 1)];

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* generate primes until we reach the nth term */
    while (cache_len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = 4 * k * k - 10 * k + 7;   /* 4*k^2 - 10*k + 7 */

        if (isPrimeGeneric(val)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = val;          /* store new prime */
        }
        ++next_k;
    }

    return cache[(size_t)(n - 1)];
}
