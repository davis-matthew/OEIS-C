/*
 * A130174  a(n) = n-1 + (total number of digits in a(1), ..., a(n-1)).
 * https://oeis.org/A130174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + 1 + A055642(a(n-1)) for n>1, a(1) = 0 where
 *            A055642(x) is the number of decimal digits in x
 * Range:     u8 n <= 74, u16 n <= 9270, u32 n <= 9270, u64 n <= 9270,
 *            u128 n <= 9270, big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"   /* provides int digit_len(seq_t) */

seq_t a(idx_t n)
{
    /* Static cache: cache[0] holds a(1), cache[1] holds a(2), ...   */
    static seq_t *cache      = NULL;   /* dynamic array of terms   */
    static size_t cap        = 0;      /* allocated capacity        */
    static size_t size       = 0;      /* number of terms stored    */

    if (n == 0)   /* undefined for n<1; return 0 as a sane default   */
        return (seq_t)0;

    /* Convert idx_t to size_t for array indexing (size_t is 64‑bit on all
       builds that use 64‑bit indices).                                   */
    size_t needed = (size_t)n;

    /* Grow the cache if necessary.  We double the capacity on each
       expansion to keep the amortised cost O(1) per insertion.       */
    if (cap < needed) {
        size_t new_cap = cap ? cap << 1 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0)   /* allocation failed */
            abort();                /* cannot recover */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to index n (1‑based => array index = n-1). */
    for (size_t i = size; i < needed; ++i) {
        if (i == 0) {                   /* a(1) = 0 */
            cache[0] = (seq_t)0;
        } else {
            seq_t prev = cache[i-1];
            seq_t inc  = (seq_t)1 + (seq_t)digit_len(prev);
            cache[i]   = prev + inc;
        }
    }
    size = needed;

    /* Return the requested term. */
    return cache[n-1];
}
