/*
 * A015508  a(1) = 1, a(n) = Sum_{k=1..n-1} ((7^k - 1)/6)*a(k).
 * https://oeis.org/A015508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((7^(n-1)+5)/6) * a(n-1), a(1)=1, a(2)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t capacity  = 0;   /* allocated size of cache          */
    static idx_t current   = 0;   /* number of terms stored (a(1)...a(current)) */

    /* 7^(current)   (current >= 2 after initialisation) */
    static seq_t pow7 = (seq_t)1;

    if (n < 1)
        return (seq_t)0;  /* undefined for n = 0 */

    /* Initialise cache for a(1) and a(2) on first call */
    if (current < 2) {
        idx_t init_cap = 2;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)1;          /* a(2) */
        current = 2;
        capacity = init_cap;
        pow7 = (seq_t)49;             /* 7^2, ready for a(3) */
    }

    /* Already computed? */
    if (n <= current)
        return cache[n-1];

    /* Grow cache if necessary */
    if (n > capacity) {
        idx_t new_cap = capacity ? capacity : 1;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = current + 1; i <= n; ++i) {
        /* i is 1‑based; -1 shifts to 0‑based index */
        seq_t factor = (pow7 + (seq_t)5) / (seq_t)6;
        cache[i-1]   = factor * cache[i-2];
        pow7 *= (seq_t)7;     /* advance 7^(i) for next iteration */
    }
    current = n;
    return cache[n-1];
}
