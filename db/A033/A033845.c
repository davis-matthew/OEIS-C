/*
 * A033845  Numbers k of the form 2^i*3^j, where i and j >= 1.
 * https://oeis.org/A033845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * h(n) where h(n) is the n‑th 3‑smooth number (2^i ·
 *            3^j with i,j≥0)
 * Range:     u8 n <= 14, u16 n <= 68, u32 n <= 297, u64 n <= 1240,
 *            u128 n <= 5064, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates 3‑smooth numbers using the two‑pointer method and scales by 6. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

/* sequence type and index type are supplied by the build environment */
seq_t a(idx_t n)
{
    /* Static cache of 3‑smooth numbers  (h[0] = 1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached elements */
    static size_t cache_cap = 0;   /* allocated size of the cache */
    static size_t p2 = 0, p3 = 0;  /* pointers used for *2 and *3 */

    /* Ensure the cache holds at least n elements. */
    if (n > cache_len) {
        /* Resize cache if necessary. */
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise the first element when the cache is empty. */
        if (cache_len == 0) {
            cache[0] = 1;        /* 2^0 * 3^0 */
            cache_len = 1;
        }

        /* Generate more 3‑smooth numbers. */
        while (cache_len < n) {
            seq_t v2 = cache[p2];
            seq_t v3 = cache[p3];
            seq_t t2 = mul_small(v2, 2);   /* v2 * 2 */
            seq_t t3 = mul_small(v3, 3);   /* v3 * 3 */
            seq_t next = (t2 < t3) ? t2 : t3;
            cache[cache_len++] = next;
            if (t2 == next) ++p2;
            if (t3 == next) ++p3;
        }
    }

    /* Return 6 times the (n‑1)-th 3‑smooth number. */
    seq_t result = mul_small(cache[n - 1], 6);
    return result;
}
