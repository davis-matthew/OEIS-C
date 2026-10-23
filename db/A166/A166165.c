/*
 * A166165  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) - 34a(n-1) - 34a(n-2)
 *            - … - 34a(n-9) + 595a(n-10) = p(n), where p(0)=1, p(1)…p(9)=2,
 *            p(10)=1, and p(n)=0 for n>10. Initial values (offset 0): a(0)=1,
 *            a(1)=36, a(2)=1260, a(3)=44100, a(4)=1543500, a(5)=54022500,
 *            a(6)=1890787500, a(7)=66177562500, a(8)=2316...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Memoisation table */
    static seq_t *cache = NULL;     /* computed terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t max_computed = 0;  /* highest index already filled */

    /* First 10 terms */
    static const seq_t base[10] = {
        (seq_t)1,
        (seq_t)36,
        (seq_t)1260,
        (seq_t)44100,
        (seq_t)1543500,
        (seq_t)54022500,
        (seq_t)1890787500,
        (seq_t)66177562500,
        (seq_t)2316214687500,
        (seq_t)81067514062500
    };

    /* Return early for the first ten indices */
    if (n < (idx_t)10)
        return base[(size_t)n];

    /* Initialise cache on the first call */
    if (cache == NULL) {
        cache_cap   = 10;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 10; ++i)
            cache[i] = base[i];
        max_computed = 9;
    }

    /* Grow the array if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int j = 1; j <= 9; ++j)
            sum += cache[(size_t)(k - j)];
        seq_t term = sum * (seq_t)34;
        term -= cache[(size_t)(k - 10)] * (seq_t)595;
        /* Add the non‑homogeneous part p(k) */
        if (k == 10)               /* p(10) = 1 */
            term += (seq_t)1;
        /* For k < 10 the values are already in base, never reached here */
        cache[(size_t)k] = term;
    }

    max_computed = n;
    return cache[(size_t)n];
}
