/*
 * A143389  Coefficient Expansion sequence of a Weaver Morse Code polynomial
 *          (using Cyclotomic prime base dot, dash, letter space and word
 *          space symbols): p(x) = -5 - 10 x - 12 x^2 - 10 x^3 - 7 x^4 - 3 x^5
 *          + 5 x^7 + 8 x^8 + 9 x^9 + 8 x^10 + 6 x^11 + 3 x^12 + x^13.
 * https://oeis.org/A143389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 33, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -3*a(n-1)-6*a(n-2)-8*a(n-3)-9*a(n-4)-8*a(n-5)-5*a(n-6)
                 +3*a(n-8)+7*a(n-9)+10*a(n-10)+12*a(n-11)+10*a(n-12)+5*a(n-13)
   for n ≥ 13; initial values:
   a(0)=1, a(1)=-3, a(2)=3, a(3)=1, a(4)=-6, a(5)=7, a(6)=-1,
   a(7)=-9, a(8)=11, a(9)=7, a(10)=-34, a(11)=32, a(12)=23 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values a(0) … a(12) */
    static const seq_t init[13] = {
        1, -3, 3, 1, -6, 7, -1, -9, 11, 7, -34, 32, 23
    };

    /* Cache for already computed terms */
    static seq_t *cache      = NULL;   /* dynamic array of seq_t */
    static idx_t  cache_len  = 0;      /* number of terms stored   */
    static size_t cache_cap  = 0;      /* array capacity           */

    /* Initialise cache on the first call */
    if (!cache) {
        /* Start with a capacity that covers the initial terms */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed – abort quietly */
        for (idx_t i = 0; i < 13; ++i) cache[i] = init[i];
        cache_len = 13;
    }

    /* If the requested index is already computed */
    if (n < cache_len) return cache[(size_t)n];

    /* Grow cache if necessary and compute missing terms */
    while (cache_len <= n) {
        /* Grow capacity if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* allocation failed – abort quietly */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute the next term using the recurrence */
        idx_t i = cache_len;              /* index of the term to compute */
        seq_t next = -3 * cache[i - 1]
                     -6 * cache[i - 2]
                     -8 * cache[i - 3]
                     -9 * cache[i - 4]
                     -8 * cache[i - 5]
                     -5 * cache[i - 6]
                     +3 * cache[i - 8]
                     +7 * cache[i - 9]
                     +10 * cache[i - 10]
                     +12 * cache[i - 11]
                     +10 * cache[i - 12]
                     +5 * cache[i - 13];
        cache[cache_len++] = next;
    }

    return cache[(size_t)n];
}
