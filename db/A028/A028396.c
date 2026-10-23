/*
 * A028396  Iterate the map in A006369 starting at 14.
 * https://oeis.org/A028396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 14; for n > 0, * a(n) = f(a(n‑1)), where * f(x) = x - x/3
 *            if x % 3 == 0 * f(x) = x + (x + 1)/3 otherwise * (All arithmetic
 *            is on seq_t, see build configuration.) * This implements the
 *            recurrence from OEIS A028396, which * iterates the map in
 *            A006369 starting at 14
 * Range:     u8 n <= 24, u16 n <= 89, u32 n <= 202, u64 n <= 793,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* n is non‑negative, fits in size_t */

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)14;        /* a(0) = 14 */
        cache_len = 1;
    }

    /* Extend cache if n is beyond current length. */
    if (idx >= cache_len) {
        /* Grow the storage if necessary. */
        if (idx + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap <= idx)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence. */
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t prev = cache[i - 1];
            seq_t next;
            if (prev % 3 == 0)
                next = prev - prev / 3;
            else
                next = prev + (prev + 1) / 3;
            cache[i] = next;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
