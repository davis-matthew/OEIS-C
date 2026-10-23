/*
 * A044224  Numbers k such that string 4,5 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has45
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k with the octal substring “45” that do not occur in k‑1 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has45.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Static cache of terms.  A capacity of 20000 is ample for n up to ~10000. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static const size_t CACHE_CAP = 20000;
    static idx_t candidate = 1;

    if (!cache)
    {
        cache = (seq_t *)malloc(CACHE_CAP * sizeof(seq_t));
        if (!cache) abort();
    }

    while (cache_len < (size_t)n)
    {
        for (idx_t k = candidate; ; ++k)
        {
            if (has45(k) && !has45(k - 1))
            {
                cache[cache_len++] = (seq_t)k;
                candidate = k + 1;
                break;
            }
        }
    }

    return cache[(size_t)n - 1];
}
