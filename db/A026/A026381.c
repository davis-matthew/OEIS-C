/*
 * A026381  T(n,n-2), where T is the array in A026374.
 * https://oeis.org/A026381
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of z^n in
 *            z^2*(1+3z+5z^2)/[(1-z)^3*(1+z)^2]. It satisfies the recurrence
 *            for n ≥ 7: a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) +
 *            a(n-5), with initial values a(2)=1, a(3)=4, a(4)=11, a(5)=17,
 *            a(6)=30
 * Range:     u8 n <= 15, u16 n <= 242, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 1; /* last index computed */

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed, return 0 as fallback */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 2)       cache[i] = (seq_t)1;
        else if (i == 3)  cache[i] = (seq_t)4;
        else if (i == 4)  cache[i] = (seq_t)11;
        else if (i == 5)  cache[i] = (seq_t)17;
        else if (i == 6)  cache[i] = (seq_t)30;
        else {
            /* recurrence: a(n) = a(n-1) + 2*a(n-2) + a(n-5) - 2*a(n-3) - a(n-4) */
            seq_t sum1 = cache[i-1];
            sum1 += (seq_t)2 * cache[i-2];
            sum1 += cache[i-5];
            seq_t sum2 = (seq_t)2 * cache[i-3];
            sum2 += cache[i-4];
            cache[i] = sum1 - sum2;
        }
    }
    last = n;
    return cache[n];
}
