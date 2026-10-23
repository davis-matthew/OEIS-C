/*
 * A187611  Number of 7-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-bishop's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>14, with a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=0, a(5)=48, a(6)=616, a(7)=2936, a(8)=8530,
 *            a(9)=17611, a(10)=32086, a(11)=51955, a(12)=76258, a(13)=105978,
 *            a(14)=140386
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The external function required by the test harness. */
seq_t a(idx_t n)
{
    /* Base values for 1..14. */
    static const seq_t init[14] = {
        0ULL, 0ULL, 0ULL, 0ULL,
        48ULL, 616ULL, 2936ULL, 8530ULL,
        17611ULL, 32086ULL, 51955ULL,
        76258ULL, 105978ULL, 140386ULL
    };

    /* Fast return for the initial segment. */
    if (n <= 14)
        return init[n - 1];

    /* Static cache for computed terms. */
    static seq_t *cache    = NULL;          /* array of terms */
    static size_t cache_cap = 0; /* allocated capacity (>= n+1) */
    static size_t max_computed = 0; /* highest n that has been computed */

    /* Allocate space if needed. */
    if (cache == NULL || (size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) {
            /* Memory allocation failed – the test harness does not
               expect failure, but we return 0 as a safe default. */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;

        /* Initialise the known terms if not already done. */
        if (max_computed < 14) {
            size_t i;
            for (i = 1; i <= 14; ++i)
                cache[i] = init[i - 1];
            max_computed = 14;
        }
    }

    /* Compute missing terms using the recurrence. */
    if (n > max_computed) {
        idx_t i;
        for (i = max_computed + 1; i <= n; ++i) {
            seq_t t = cache[i - 1] * 3;    /* 3*a(i-1) */
            t += cache[i - 3];            /* + a(i-3)   */
            t -= cache[i - 2] * 3;        /* - 3*a(i-2) */
            cache[i] = t;
        }
        max_computed = n;
    }

    return cache[n];
}
