/*
 * A234228  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A234228:
   Number of (n+1) × 2 arrays over {0,…,7} with every 2×2 subblock
   having diagonal sum differing from its antidiagonal sum by 3.
   Empirical recurrence (for n ≥ 7):
   a(n) = 182 a(n−2) – 9024 a(n−4) + 66960 a(n−6)
   Initial values (offset 1):
   a(1)=568, a(2)=5260, a(3)=48592,
   a(4)=449880, a(5)=4160672, a(6)=38547120
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values (six terms) */
    static const seq_t init[6] = {
        (seq_t)568,
        (seq_t)5260,
        (seq_t)48592,
        (seq_t)449880,
        (seq_t)4160672,
        (seq_t)38547120
    };

    /* Cache of computed terms; grows on demand */
    static seq_t *cache = NULL;
    static idx_t computed = 0;        /* number of terms stored in cache */

    if (n <= 6)
        return init[n - 1];

    /* Ensure cache has space for n terms */
    if (computed < n) {
        /* Reallocate cache to required size */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – unlikely */
        cache = tmp;

        /* If cache was empty, load the six initial terms */
        if (computed == 0) {
            for (idx_t i = 0; i < 6; ++i)
                cache[i] = init[i];
            computed = 6;
        }

        /* Compute remaining terms using the recurrence */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* a(i) = 182*a(i-2) – 9024*a(i-4) + 66960*a(i-6) */
            seq_t val = cache[i - 3] * (seq_t)182;   /* a(i-2) */
            val += cache[i - 7] * (seq_t)66960;      /* a(i-6) */
            val -= cache[i - 5] * (seq_t)9024;       /* a(i-4) */
            cache[i - 1] = val;
        }
        computed = n;
    }

    return cache[n - 1];
}
