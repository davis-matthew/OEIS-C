/*
 * A179819  Monetary amounts in U.S. cents such that the smallest number of
 *          coins summing to the amount uses only denominations of 10 cents
 *          and above.
 * https://oeis.org/A179819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4) for n >= 5 initial values: a(1)=10,
 *            a(2)=20, a(3)=25, a(4)=35 (equivalently a(n) = a(n-12)+100 for n
 *            > 12)
 * Range:     u8 n <= 30, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms, 1‑based indexing */
    static size_t cap    = 0;     /* allocated capacity of cache */
    static idx_t max_computed = 0;/* highest n for which cache[n] is valid */

    if (n < 1)                 /* undefined for n < 1, return 0 */
        return 0;

    /* Ensure enough space in the cache */
    size_t need = (size_t)n + 1;     /* need entries 0..n */
    if (cap < need) {
        size_t new_cap = cap ? cap * 2 : 16;   /* at least 16 entries */
        while (new_cap < need) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute any terms up to n that we have not yet computed */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)10;
        else if (i == 2)
            cache[2] = (seq_t)20;
        else if (i == 3)
            cache[3] = (seq_t)25;
        else if (i == 4)
            cache[4] = (seq_t)35;
        else
            cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
    }

    max_computed = n;
    return cache[n];
}
