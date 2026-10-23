/*
 * A117666  Expansion of (1-3*x+x^2)*(1-x-x^2)/((1+x+x^2)*(1-x+x^2)*(1-x)^2).
 * https://oeis.org/A117666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 380, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-2, a(2)=-3, a(3)=2, a(4)=3, a(5)=2
   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) - a(n-6)   (n>5) */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Array large enough for the expected test range (≤10\,000). */
    static seq_t cache[20000];
    static size_t computed = 0;   /* next index that needs to be computed */

    /* On first call, initialise the first six terms. */
    if (computed == 0) {
        cache[0] = 1;
        cache[1] = -2;
        cache[2] = -3;
        cache[3] = 2;
        cache[4] = 3;
        cache[5] = 2;
        computed = 6;              /* indices 0..5 are ready */
    }

    /* If the requested index is already computed, return it. */
    if ((size_t)n < computed)
        return cache[(size_t)n];

    /* Compute terms up to n. */
    while (computed <= (size_t)n) {
        cache[computed] =
            2 * cache[computed - 1] - 2 * cache[computed - 2]
          + 2 * cache[computed - 3] - 2 * cache[computed - 4]
          + 2 * cache[computed - 5] -     cache[computed - 6];
        ++computed;
    }

    return cache[(size_t)n];
}
