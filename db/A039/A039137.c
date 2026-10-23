/*
 * A039137  Numbers whose base-10 representation has the same number of 3's
 *          and 6's.
 * https://oeis.org/A039137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_3_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A039137: numbers whose decimal representation
   contains an equal number of digits 3 and 6.  The routine
   generates numbers on demand using a simple brute‑force search
   and caches the results. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_equal_3_6.h"

/* Cache for computed sequence values */
#if defined(__cplusplus)
static seq_t *cache = nullptr;
#else
static seq_t *cache = NULL;
#endif
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Grow the cache array to accommodate at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;

#if defined(__cplusplus)
    /* C++: use new[] / delete[] to handle non‑trivial types */
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    delete [] cache;
    cache = tmp;
#else
    /* C: use realloc (seq_t is assumed trivially copy‑able) */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    cache = tmp;
#endif

    cache_cap = new_cap;
}

/* Public function computing the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)    /* undefined, but return 0 */
        return 0;
    size_t target = (size_t)n;

    if (target > cache_size) {
        /* Generate terms until we have enough. */
        seq_t start = (cache_size == 0) ? 0 : cache[cache_size - 1] + 1;

        while (cache_size < target) {
            /* Find the next number satisfying the property. */
            seq_t cand = start;
            while (!has_equal_3_6(cand))
                ++cand;

            start = cand + 1;

            ensure_capacity(cache_size + 1);
            cache[cache_size++] = cand;
        }
    }

    return cache[n - 1];
}
