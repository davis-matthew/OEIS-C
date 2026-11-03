/*
 * A102545  Numbers k such that k999999 is prime.
 * https://oeis.org/A102545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Numbers k such that k999999 is prime. The returned value a(n) is
 *            the n-th integer k (offset 1)
 * Range:     u8 n <= 30, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* dynamic array helpers -------------------------------------------------- */
static seq_t *resize_array(seq_t *old, idx_t old_size, idx_t new_size)
{
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_size]();        /* value–init for C++ objects   */
#else
    new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));   /* zero for C */
#endif
    /* copy old data */
    idx_t i;
    for (i = 0; i < old_size && i < new_size; ++i)
        new_arr[i] = old[i];

    /* free old storage */
    if (old) {
#ifdef __cplusplus
        delete[] old;
#else
        free(old);
#endif
    }
    return new_arr;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static idx_t cache_capacity = 0;
    static idx_t last_k = 0;       /* last k that was examined */

    if (n == 0) return (seq_t)0; /* undefined, but safe */

    /* Grow cache until we have at least n terms */
    while (cache_size < n)
    {
        /* Expand cache storage if necessary */
        if (cache_size >= cache_capacity)
        {
            idx_t new_capacity = cache_capacity == 0 ? 1 : cache_capacity << 1;
            cache = resize_array(cache, cache_capacity, new_capacity);
            cache_capacity = new_capacity;
        }

        /* Look for next k */
        ++last_k;
        uint64_t cand = ((uint64_t)last_k * 1000000ULL) + 999999ULL;
        if (is_prime_u64(cand))
        {
            cache[cache_size++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
