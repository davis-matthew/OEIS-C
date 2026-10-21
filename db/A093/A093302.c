/*
 * A093302  a(n) = (a(n-1)+(2n-1))*(2n) with a(0) = 0.
 * https://oeis.org/A093302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(n) = (a(n‑1) + (2*n - 1)) * (2*n) */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system.  They support the basic
   arithmetic and comparison operations used below. */
seq_t a(idx_t n)
{
    /* static cache to allow repeated calls with arbitrary n order */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;  /* number of elements stored (cache[0]..cache[n]) */

    /* If n is 0, just return 0 (ensure cache[0] is set). */
    if (n == 0) {
        if (!cache) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            cache[0] = 0;
            cached_size = 1;
        }
        return cache[0];
    }

    /* Extend cache if necessary */
    if (n >= cached_size) {
        size_t new_size = (size_t)(n + 1);
        if (!cache) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            cache[0] = 0;
            cached_size = 1;
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }

        /* Compute missing terms */
        for (idx_t i = cached_size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t two_n = (seq_t)i * (seq_t)2;
            seq_t addend = two_n - (seq_t)1;     /* 2*n - 1 */
            seq_t tmp = prev + addend;           /* a(i-1) + (2*i - 1) */
            cache[i] = tmp * two_n;              /* (prev + addend) * 2*i */
        }
        cached_size = new_size;
    }

    return cache[n];
}
