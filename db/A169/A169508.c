/*
 * A169508  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*14^(n-1) for n = 0..33; for n >= 34, the terms satisfy
 *            the linear recurrence a(n) = 13*(a(n-1)+...+a(n-33)) -
 *            91*a(n-34). This recurrence follows from the rational generating
 *            function that appears in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements allocated   */
    static idx_t last_computed = 33;  /* highest n for which cache[i] is known */
    static bool init = false;
    static seq_t sum_window = 0;     /* sum of a[i-1] .. a[i-33]  */

    /* initialise cache and first 34 terms once  */
    if (!init) {
        /* allocate space for indices 0..33 at start */
        cache_size = 34;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));

        /* a(0) = 1 */
        cache[0] = (seq_t)1;

        /* generate a(1)..a(33) = 15*14^(i-1)  */
        seq_t pow_14 = (seq_t)1;
        sum_window = (seq_t)0;
        for (idx_t i = 1; i <= 33; ++i) {
            cache[i] = (seq_t)15 * pow_14;
            pow_14 *= (seq_t)14;
            sum_window += cache[i];           /* sum of a[1]..a[33] */
        }

        /* last_computed already 33 */
        init = true;
    }

    /* enlarge cache if the requested index is beyond current allocation */
    if (n >= cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        cache_size = n + 1;
    }

    /* expand memoisation to cover the requested n */
    while (last_computed < n) {
        ++last_computed;
        if (last_computed <= 33) {
            /* already initialised – nothing to do */
            continue;
        }

        /* recurrence: a(k) = 13*sum_window - 91*a(k-34) */
        seq_t val = (seq_t)13 * sum_window - (seq_t)91 * cache[last_computed - 34];
        cache[last_computed] = val;

        /* update sliding window sum for next iteration
           new sum = old sum + new value - value that leaves the window */
        sum_window += val;
        sum_window -= cache[last_computed - 33];
    }

    return cache[n];
}
