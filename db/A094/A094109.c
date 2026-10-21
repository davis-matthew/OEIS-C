/*
 * A094109  Start with "I" in Roman numerals (one). The next number (in Roman
 *          numerals) describes the number of Roman numerals written
 *          previously in the sequence. Convert this infinite sequence into an
 *          infinite sequence of Arabic numbers.
 * https://oeis.org/A094109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = a(2) = 1, a(n) = a(n-1) + A006968(a(n-1)) (A006968 =
 *            Roman‑numeral length)
 * Range:     u8 n <= 37, u16 n <= 573, u32 n <= 573, u64 n <= 573,
 *            u128 n <= 573, big any n
 * Helpers:   roman_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/roman_len.h"

/* Cache for sequence values.  cache[1] and cache[2] are defined. */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined below offset 1 */

    /* Allocate the cache if it's the first call or needs to grow */
    if (!cache) {
        size_t sz = (size_t)n + 1;
        if (sz < 3) sz = 3;          /* space for indices 1 and 2 */
        cache = (seq_t *)realloc(NULL, sz * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_size = 2;
    }

    /* Grow the cache and compute missing terms */
    if (n > cache_size) {
        size_t newsize = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure */

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            int   len  = roman_len(prev);
            cache[i] = prev + (seq_t)len;
        }
        cache_size = n;
    }

    return cache[n];
}
