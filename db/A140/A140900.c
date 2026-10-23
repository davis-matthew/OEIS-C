/*
 * A140900  A nonnegative integer n is included if the binary representation
 *          of n and the digit-reversal (with leading 0's) of the binary
 *          representation of n do not have any 1's in the same position.
 * https://oeis.org/A140900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth nonnegative integer m satisfying (m &
 *            reverse_bits(m)) == 0 (OEIS A140900). This implementation
 *            searches incrementally and caches results
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   revbits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/revbits.h"

/* Cached results of the sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static seq_t next_candidate = 0;

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    idx_t idx = n - 1;            /* 0‑based offset for cache lookup */
    if (idx < 0)
        return 0;                 /* undefined for n < 1 */

    /* Extend cache until it contains the requested index. */
    while (cache_size <= (size_t)idx)
    {
        seq_t cand = next_candidate;
        seq_t rev  = revbits(cand);
        if ((cand & rev) == 0)    /* satisfies the property */
        {
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        ++next_candidate;
    }

    return cache[(size_t)idx];
}
