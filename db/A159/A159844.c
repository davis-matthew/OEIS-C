/*
 * A159844  Positive numbers y such that y^2 is of the form x^2+(x+359)^2 with
 *          integer x.
 * https://oeis.org/A159844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n>6; a(1)=325, a(2)=359, a(3)=401,
 *            a(4)=1549, a(5)=1795, a(6)=2081
 * Range:     u8 none, u16 n <= 11, u32 n <= 30, u64 n <= 67, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS sequence A159844.
   The function uses a simple linear recurrence and memoizes
   previously computed values in a static array. */
seq_t a(idx_t n)
{
    /* Handle the known initial terms directly. */
    if (n <= 6) {
        switch (n) {
            case 1: return (seq_t)325;
            case 2: return (seq_t)359;
            case 3: return (seq_t)401;
            case 4: return (seq_t)1549;
            case 5: return (seq_t)1795;
            case 6: return (seq_t)2081;
        }
    }

    /* Static cache of computed terms: cache[i] == a(i) for i >= 1. */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size (in number of elems) */
    static idx_t computed = 0;      /* highest index that has been computed */

    /* Allocate initial cache if not yet done. */
    if (cache == NULL) {
        if (cap == 0) cap = 16;    /* start with some room */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Ensure capacity for index n. */
    if (cap <= (size_t)n) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* If we have not yet computed up to (n-1), do so. */
    if (computed < n) {
        /* Initialise the first six terms the first time. */
        if (computed == 0) {
            cache[1] = (seq_t)325;
            cache[2] = (seq_t)359;
            cache[3] = (seq_t)401;
            cache[4] = (seq_t)1549;
            cache[5] = (seq_t)1795;
            cache[6] = (seq_t)2081;
            computed = 6;
        }
        /* Compute subsequent terms using the recurrence. */
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6];
        }
        computed = n;
    }

    return cache[n];
}
