/*
 * A232132  Number of (n+1)X(3+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A232132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A232132
   Empirical recurrence: a(n) = 65*a(n-1) -588*a(n-2) +2304*a(n-3) -5156*a(n-4)
                            + 7136*a(n-5) -5632*a(n-6) +2048*a(n-7) -256*a(n-8)
   Initial values for n = 1 … 8: 200, 10956, 602468, 33162868,
                                 1825568436, 100495188564, 5532131001812,
                                 304536704087316 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Initial eight terms, as given in the OEIS entry. */
    static const seq_t init[8] = {
        (seq_t)200ULL,
        (seq_t)10956ULL,
        (seq_t)602468ULL,
        (seq_t)33162868ULL,
        (seq_t)1825568436ULL,
        (seq_t)100495188564ULL,
        (seq_t)5532131001812ULL,
        (seq_t)304536704087316ULL
    };

    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;      /* array of cached terms */
    static size_t  cap   = 0;        /* allocated capacity of array  */
    static size_t  len   = 0;        /* number of valid terms stored   */

    /* If the requested term is among the first eight, return it. */
    if (n <= 8)
        return init[n - 1];

    /* Initialise the cache on first use. */
    if (!cache) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        len = 8;
    }

    /* Increase the capacity if necessary. */
    if ((size_t)n > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n)
            newcap = (size_t)n;
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to the requested index. */
    if ((size_t)n > len) {
        /* Recurrence coefficients. */
        const int coef[8] = { 65, -588,  2304, -5156,
                               7136, -5632, 2048, -256 };

        for (size_t k = len + 1; k <= (size_t)n; ++k) {
            seq_t sum = 0;
            for (int i = 0; i < 8; ++i) {
                /* term a(k-i-1) is at index k-2-i of cache */
                size_t idx    = k - 2 - (size_t)i;
                seq_t term    = cache[idx];
                int  c        = coef[i];
                unsigned int abscoef = (c < 0) ? (unsigned int)(-c) : (unsigned int)c;
                seq_t prod = term * (seq_t)abscoef;
                if (c < 0)
                    sum = sum - prod;
                else
                    sum = sum + prod;
            }
            cache[k - 1] = sum;
        }
        len = n;
    }

    return cache[n - 1];
}
