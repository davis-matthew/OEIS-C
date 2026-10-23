/*
 * A025797  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^8)).
 * https://oeis.org/A025797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)-a(n-5)+a(n-8)-a(n-10)-a(n-11)+a(n-13), with
 *            a(k)=0 for k<0 and a(0)=1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache for previously computed values. */
    static seq_t *cache = (seq_t *)NULL;
    static idx_t cached_max = 0;          /* largest n for which cache[n] is computed */

    /* Initialise cache with the zeroth term. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();              /* allocation failure */
        cache[0] = (seq_t)1;
        cached_max = 0;
    }

    /* Grow the cache up to the requested index, if necessary. */
    if (n > cached_max) {
        size_t new_cap = (size_t)n + 1;   /* capacity in terms of number of elements */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* Compute new values using the linear recurrence. */
        for (idx_t k = cached_max + 1; k <= n; ++k) {
            long long val = 0;            /* signed intermediate to allow subtraction */

            if (k >= 2) val += (long long)cache[k - 2];
            if (k >= 3) val += (long long)cache[k - 3];
            if (k >= 5) val -= (long long)cache[k - 5];
            if (k >= 8) val += (long long)cache[k - 8];
            if (k >=10) val -= (long long)cache[k -10];
            if (k >=11) val -= (long long)cache[k -11];
            if (k >=13) val += (long long)cache[k -13];

            cache[k] = (seq_t)val;
        }
        cached_max = n;
    }

    return cache[n];
}
