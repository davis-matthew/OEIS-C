/*
 * A238712  Numbers in which squares may end (in base 10).
 * https://oeis.org/A238712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest non‑negative integer that is a
 *            quadratic residue modulo some power of 10. The function builds a
 *            union of square residues for 10¹, 10², … until it contains at
 *            least n distinct numbers
 * Range:     u8 n <= 24, u16 n <= 391, u32 n <= 1322, u64 n <= 9306,
 *            u128 n <= 9306, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Guard against the undefined 0‑index */
    if (n == 0)
        return 0;

    /* Static cache of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Current power of ten (10^k).  Start with 10¹. */
    static idx_t current_mod = 10;

    /* Bitmap of all residues seen so far.  Entry i == 1 means i is a
       quadratic residue modulo some 10^k. */
    static uint8_t *bitmap = NULL;
    static size_t bitmap_size = 0;          /* number of bytes allocated */

    /* If we already have enough terms, return the cached value. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Grow the sequence until it contains at least n terms. */
    for (;;) {
        idx_t mod = current_mod;

        /* Ensure the bitmap is large enough. */
        if (mod > bitmap_size) {
            size_t new_size = (size_t)mod;
            uint8_t *tmp = (uint8_t *)realloc(bitmap, new_size);
            if (!tmp)
                return 0;                     /* allocation failure (unlikely) */
            if (new_size > bitmap_size)
                memset(tmp + bitmap_size, 0, new_size - bitmap_size);
            bitmap     = tmp;
            bitmap_size = new_size;
        }

        /* Mark all residues of squares modulo the current modulus. */
        for (idx_t m = 0; m < mod; ++m) {
            idx_t r = (idx_t)((((seq_t)m * (seq_t)m) % (seq_t)mod));
            bitmap[r] = 1;
        }

        /* Rebuild the cache array from the bitmap. */
        size_t count = 0;
        for (idx_t i = 0; i < bitmap_size; ++i)
            if (bitmap[i]) ++count;

        seq_t *tmp_cache = (seq_t *)realloc(cache, count * sizeof(seq_t));
        if (!tmp_cache)
            return 0;  /* allocation failure */
        cache     = tmp_cache;
        cache_len = count;

        size_t pos = 0;
        for (idx_t i = 0; i < bitmap_size; ++i)
            if (bitmap[i])
                cache[pos++] = (seq_t)i;

        /* If we now have enough terms, return the requested element. */
        if (cache_len >= (size_t)n)
            return cache[n - 1];

        /* Prepare the next modulus, unless it would overflow. */
        if (current_mod > (UINT64_MAX / 10))
            break;                 /* should not happen for n up to 10000 */
        current_mod *= 10;
    }

    return 0;                       /* should never happen */
}
