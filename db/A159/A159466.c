/*
 * A159466  Positive numbers y such that y^2 is of the form x^2 + (x+127)^2
 *          with integer x.
 * https://oeis.org/A159466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 31, u64 n <= 69, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A159466
 *
 *  a(n) = 6*a(n-3) - a(n-6)   for n > 6
 *  a(1)=113, a(2)=127, a(3)=145, a(4)=533, a(5)=635, a(6)=757
 */

#include <stdlib.h>
#include <string.h>

/* Cache for computed values: 1‑based index.
 * cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size (excluding dummy 0 index) */
static size_t cache_len = 0;   /* highest index that has been computed */

/* Return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Handle the first six values immediately. */
    if (n <= 6) {
        static const seq_t init[7] = {0, 113, 127, 145, 533, 635, 757};
        return init[(size_t)n];
    }

    /* Ensure that the cache is large enough. */
    if (!cache || (size_t)n > cache_cap) {
        size_t old_cap = cache_cap;
        if (cache_cap == 0) cache_cap = 8;    /* initial allocation size */
        while (cache_cap <= (size_t)n) cache_cap <<= 1;  /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failed – abort program */
        cache = tmp;

        /* Zero the newly allocated entries.  This assignment works for
         * arbitrary‑precision integer classes as long as they support
         * assignment from 0. */
        for (size_t i = old_cap + 1; i <= cache_cap; ++i) cache[i] = 0;
    }

    /* Fill in the initial six terms if they are missing. */
    if (cache_len < 6) {
        static const seq_t init[7] = {0, 113, 127, 145, 533, 635, 757};
        size_t start = cache_len + 1;
        if (start == 0) start = 1;
        size_t end   = 6;
        if (end > (size_t)n) end = (size_t)n;
        for (size_t i = start; i <= end; ++i) cache[i] = init[i];
        if (cache_len < 6) cache_len = 6;
    }

    /* Compute values using the recurrence until we reach n. */
    if (cache_len < (size_t)n) {
        size_t i = cache_len + 1;
        if (i < 7) i = 7;                /* recurrence starts at index 7 */
        for (; i <= (size_t)n; ++i) {
            cache[i] = 6 * cache[i - 3] - cache[i - 6];
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
