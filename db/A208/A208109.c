/*
 * A208109  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 8*a(n-3) + 61*a(n-4)
                      - 66*a(n-5) - 48*a(n-6) - 252*a(n-8) + 216*a(n-9)
   for n ≥ 10, with initial values
   a(1)=9, a(2)=81, a(3)=171, a(4)=361, a(5)=1805,
   a(6)=9025, a(7)=25555, a(8)=72361, a(9)=292941. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0 is unused (offset 1) */
    if (n == 0)
        return (seq_t)0;

    /* dynamic memoisation cache */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* current size (indices 0 .. cap-1) */

    /* initial values a(1) .. a(9) */
    static const seq_t init[10] = {
        0ULL,          /* index 0 (unused) */
        9ULL, 81ULL, 171ULL, 361ULL,
        1805ULL,       /* a(5) */
        9025ULL,       /* a(6) */
        25555ULL,      /* a(7) */
        72361ULL,      /* a(8) */
        292941ULL      /* a(9) */
    };

    /* remember old size before resizing */
    size_t old_cap = cap;

    /* enlarge cache if necessary */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;          /* need indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                           /* allocation failed */
        cache = tmp;
        cap   = new_cap;
        /* initialise the new portion to zero */
        for (size_t j = old_cap; j < new_cap; ++j)
            cache[j] = (seq_t)0;
    }

    /* initialise to the first 9 terms if they are still zero */
    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        if (i <= 9) {
            if (cache[i] == (seq_t)0)
                cache[i] = init[(size_t)i];
        } else {
            /* compute a(i) from the recurrence using only seq_t operations
               which are well defined for both integral and arbitrary‑precision
               types.  All intermediate values stay non‑negative, so subtraction
               is safe. */
            seq_t pos = cache[i-1] * (seq_t)2
                      + cache[i-3] * (seq_t)8
                      + cache[i-4] * (seq_t)61
                      + cache[i-9] * (seq_t)216;
            seq_t neg = cache[i-5] * (seq_t)66
                      + cache[i-6] * (seq_t)48
                      + cache[i-8] * (seq_t)252;
            cache[i] = pos - neg;
        }
    }

    return cache[n];
}
