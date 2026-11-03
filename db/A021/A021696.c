/*
 * A021696  Decimal expansion of 1/692.
 * https://oeis.org/A021696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊10·{10ⁿ/692}⌋ (the nth decimal digit of 1/692, n≥0)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached digits */
    static idx_t  cache_len = 0;   /* number of stored digits (size of cache) */
    static seq_t  rem = 1;         /* remainder before computing next digit */

    /* Expand cache if needed */
    if (cache_len <= n) {
        /* Allocate or reallocate the cache */
        seq_t *new_cache = NULL;
        if (cache == NULL) {
            new_cache = mem_new(seq_t, (size_t)(n + 1));
            if (!new_cache) return 0;  /* out of memory; return 0 as a safe fallback */
        } else {
            new_cache = mem_resize(seq_t, cache, (size_t)(n + 1));
            if (!new_cache) return 0;  /* out of memory; return 0 as a safe fallback */
        }
        cache = new_cache;

        /* Compute digits from current cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            uint64_t tmp = (uint64_t)rem * 10ULL;       /* avoids overflow */
            seq_t digit = (seq_t)(tmp / 692ULL);        /* integer division */
            cache[i] = digit;
            rem = (seq_t)(tmp % 692ULL);                /* remainder for next digit */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
