/*
 * A196574  Number of n X 4 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,2,3,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +9*a(n-2) +39*a(n-3) +20*a(n-4) +12*a(n-5)
 *            +32*a(n-6) -17*a(n-7) +a(n-8) -4*a(n-9) +a(n-10); offset = 1
 *            Initial values: a(1)=1, a(2)=7, a(3)=37, a(4)=145, a(5)=971,
 *            a(6)=4787, a(7)=24837, a(8)=134167, a(9)=700787, a(10)=3689127
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑indexed cache of computed terms */
static size_t cacheSize = 0;      /* current allocated size (indices 0..cacheSize-1) */

static const seq_t initVals[] = {
    (seq_t)0,          /* dummy for index 0 */
    (seq_t)1,  (seq_t)7,  (seq_t)37,  (seq_t)145, (seq_t)971,
    (seq_t)4787, (seq_t)24837, (seq_t)134167, (seq_t)700787, (seq_t)3689127
};

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n >= cacheSize) {
        size_t newSize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!tmp) abort();
        /* Initialize new entries to 0 (sentinel) */
        for (size_t i = cacheSize; i < newSize; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cacheSize = newSize;
    }

    /* If already computed, return it */
    if (cache[n]) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i]) continue;          /* already known */

        if (i <= 10) {
            cache[i] = initVals[i];
            continue;
        }

        seq_t val = (seq_t)0;
        /* Positive contributions */
        val += cache[i-1] * (seq_t)2;
        val += cache[i-2] * (seq_t)9;
        val += cache[i-3] * (seq_t)39;
        val += cache[i-4] * (seq_t)20;
        val += cache[i-5] * (seq_t)12;
        val += cache[i-6] * (seq_t)32;
        val += cache[i-8];           /* coefficient 1 */
        val += cache[i-10];

        /* Negative contributions */
        val -= cache[i-7] * (seq_t)17;
        val -= cache[i-9] * (seq_t)4;

        cache[i] = val;
    }

    return cache[n];
}
