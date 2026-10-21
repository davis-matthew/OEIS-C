/*
 * A228515  Nonlinear recursion: a(n) = a(n-1) + Product_{i=1..(n-1)/2}
 *          a(n-2i+1)-a(n-2i).
 * https://oeis.org/A228515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=4 a(n) = a(n-1) + ∏_{i=1..⌊(n-1)/2⌋}
 *            (a(n-2i+1)-a(n-2i)) for n≥4 Recurrence uses earlier terms only
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t lastComputed = 0;
    static seq_t prod_even = (seq_t)1;   /* product of d(k) for even k      */
    static seq_t prod_odd  = (seq_t)1;   /* product of d(k) for odd  k      */

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t initCap = 16;            /* arbitrary small start size */
        cache = (seq_t *)malloc(sizeof(seq_t) * initCap);
        if (!cache)
            return (seq_t)0;           /* allocation failure – return 0 */

        cap = initCap;

        /* set first three values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;

        /* initialise product variables with first differences */
        prod_odd  = cache[2] - cache[1];   /* d(1) : odd index */
        prod_even = cache[3] - cache[2];   /* d(2) : even index */

        lastComputed = 3;
    }

    /* grow the cache array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newCap = (size_t)n + 1;
        if (newCap < cap * 2)
            newCap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newCap);
        if (!tmp)
            return (seq_t)0;
        cache = tmp;
        cap = newCap;
    }

    /* if already computed, simply return the stored value */
    if (n <= lastComputed)
        return cache[n];

    /* compute new terms up to n */
    for (idx_t i = lastComputed + 1; i <= n; ++i) {
        /* select the appropriate accumulated product */
        seq_t prodNeeded = (i & 1) ? prod_odd : prod_even;

        /* recurrence: a(i) = a(i-1) + prodNeeded */
        seq_t value = cache[i - 1] + prodNeeded;
        cache[i] = value;

        /* difference d(i-1) = a(i) - a(i-1)  == prodNeeded */
        seq_t d = prodNeeded;

        /* update the accumulated product for the parity of (i-1) */
        if ((i - 1) & 1)
            prod_odd *= d;   /* odd-indexed difference */
        else
            prod_even *= d;  /* even-indexed difference */
    }

    lastComputed = n;
    return cache[n];
}
