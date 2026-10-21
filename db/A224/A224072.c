/*
 * A224072  Odd odious numbers divisible by 3.
 * https://oeis.org/A224072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth odd multiple of 3 whose binary representation
 *            contains an odd number of 1‑bits (odious)
 * Range:     u8 n <= 8, u16 n <= 4368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n>=1 by definition; handle 0 as a no‑op */
    if (n == 0) return 0;

    /* Cache of previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;

    /* Next odd multiple of 3 to test */;
    static seq_t next_candidate = 3;

    /* Grow cache to the requested index */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;

        /* realloc is C-compatible; cast to seq_t* for C++ */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Scan candidates until cache is full */
        while (cache_size < new_size) {
            seq_t x = next_candidate;
            unsigned int bits = 0;

            /* Count the number of 1‑bits in x */
            while (x != 0) {
                x &= x - 1;       /* clear lowest set bit */
                ++bits;
            }

            /* Keep odd/odious numbers */
            if (bits & 1u) {
                cache[cache_size++] = next_candidate;
            }

            /* Advance to the next odd multiple of 3 (adds 6 each step) */
            next_candidate += 6;
        }
    }

    return cache[n - 1];
}
