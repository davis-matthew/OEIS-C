/*
 * A016149  Expansion of g.f. 1/((1 - 4*x)*(1 - 6*x)).
 * https://oeis.org/A016149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^(n+1) - 4^(n+1))/2. Recurrence: a(0) = 1, a(1) = 10,
 *            a(n) = 10*a(n-1) - 24*a(n-2) (n>1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms: a[0] ... a[max_computed] */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored (max index + 1) */

    /* values needed to extend the sequence efficiently:
       a_last  = a[cache_size-1]   (value at current last index)
       p4_last = 4^(cache_size-1)
       p6_last = 6^(cache_size-1) */
    static seq_t a_last, p4_last, p6_last;

    /* Initialise cache for the first call: a[0] = 1 */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory */
        cache_size = 1;
        cache[0] = (seq_t)1;
        a_last  = (seq_t)1;
        p4_last = (seq_t)1;           /* 4^0 */
        p6_last = (seq_t)1;           /* 6^0 */
    }

    /* If requested index is already cached, return immediately */
    if (n < cache_size) {
        return cache[n];
    }

    /* Extend the cache up to index n using the recurrence
       a(k+1) = 6*p6(k) + 4*a(k)
               (obtained from a(n) = 10*a(n-1)-24*a(n-2) and
                p4(k) = 4^k, p6(k) = 6^k) */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t new_a = 6 * p6_last + 4 * a_last;
        seq_t new_p4 = 4 * p4_last;
        seq_t new_p6 = 6 * p6_last;

        /* Grow the cache if necessary */
        if (cache_size == 0) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache) exit(1);          /* out of memory */
        } else {
            /* allocate one more slot */
            seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
            if (!tmp) exit(1);            /* out of memory */
            cache = tmp;
        }

        cache[cache_size] = new_a;
        cache_size++;

        /* Update the running values for next iteration */
        a_last = new_a;
        p4_last = new_p4;
        p6_last = new_p6;
    }

    return cache[n];
}
