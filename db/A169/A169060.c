/*
 * A169060  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 47*46^(n-1) for 0 < n < 24 a(n) =
 *            45*a(n-1)+45*a(n-2)+...+45*a(n-23)-1035*a(n-24) for n >= 24 a(0)
 *            = 1 OEIS A169060
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;     /* number of allocated elements */
    static idx_t  computed = 0;     /* first index not yet computed */
    static seq_t  sum23 = 0;        /* sum of the last 23 terms */

    /* Ensure cache is large enough. */
    if (n + 1 > capacity) {
        size_t new_cap = (capacity > 0) ? capacity * 2 : 32;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* If we already have the requested value, return it. */
    if (n < computed) {
        return cache[(size_t)n];
    }

    /* Compute the values from 'computed' up to 'n'. */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 47;
        } else if (i <= 23) {
            /* Multiply the previous term by 46. */
            cache[i] = cache[i - 1] * (seq_t)46;
        } else {
            /* Ensure sum23 has been initialized for i == 24. */
            if (i == 24) {
                /* Sum of a[1] … a[23] */
                seq_t s = cache[1];
                for (idx_t j = 2; j <= 23; ++j)
                    s += cache[j];
                sum23 = s;
            }
            /* a(i) = 45*sum23 - 1035*a(i-24) */
            cache[i] = (seq_t)45 * sum23 - (seq_t)1035 * cache[i - 24];
            /* Update the sliding sum for the next iteration. */
            sum23 += cache[i] - cache[i - 23];
        }
    }

    computed = n + 1;
    return cache[(size_t)n];
}
