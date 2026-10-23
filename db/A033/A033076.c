/*
 * A033076  Numbers in which all pairs of consecutive base-5 digits differ by
 *          3.
 * https://oeis.org/A033076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-3) + a(n-6) - 5*a(n-9) for n>11 Initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=9, a(6)=15, a(7)=21,
 *            a(8)=46, a(9)=78, a(10)=109, a(11)=234, a(12)=390
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 42, u64 n <= 83, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init_values[13] = {
    0,  /* dummy for index 0 */
    1,  /* a(1) */
    2,  /* a(2) */
    3,  /* a(3) */
    4,  /* a(4) */
    9,  /* a(5) */
    15, /* a(6) */
    21, /* a(7) */
    46, /* a(8) */
    78, /* a(9) */
    109,/* a(10)*/
    234,/* a(11)*/
    390 /* a(12)*/
};

seq_t a(idx_t n)
{
    /* Resolve the trivial cases directly */
    if (n <= 12)
        return _init_values[(size_t)n];

    /* Static cache storing already computed values (1‑based) */
    static seq_t *cache = NULL;      /* array of seq_t */
    static size_t capacity = 0;      /* allocated size including index 0 */
    static idx_t  cached  = 0;       /* highest index with a value computed */

    /* Grow cache if necessary */
    size_t needed = (size_t)n + 1;   /* we need indices 0 .. n */
    if (cache == NULL || needed > capacity) {
        seq_t *newc = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (newc == NULL)               /* allocation failure – abort */
            return (seq_t)0;
        cache = newc;
        capacity = needed;

        /* Only set the first 12 values when the array is freshly created */
        if (cached < 12) {
            for (idx_t i = 1; i <= 12; ++i)
                cache[(size_t)i] = _init_values[(size_t)i];
            cached = 12;
        }
    }

    /* Compute any missing terms up to n */
    if (n > cached) {
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* All arithmetic uses seq_t operators only */
            seq_t term1 = 5 * cache[(size_t)(i - 3)];
            seq_t term2 =       cache[(size_t)(i - 6)];
            seq_t term3 = 5 * cache[(size_t)(i - 9)];
            cache[(size_t)i] = term1 + term2 - term3;
        }
        cached = n;
    }

    return cache[(size_t)n];
}
