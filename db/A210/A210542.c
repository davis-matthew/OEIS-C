/*
 * A210542  Number of arrays of n nonnegative integers with value i>0
 *          appearing only after i-1 has appeared at least 5 times.
 * https://oeis.org/A210542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 1, a(4) = 1, a(5) = 1 a(n) =
 *            Σ_{i=0}^{n-5} binomial(n-5,i) * a(i) for n > 5 (a(0) is defined
 *            as 1 to support the recurrence)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 27, u64 n <= 42, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: indices 0..max_computed */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  max_computed = 0;   /* highest index computed (≥0) */

    /* allocate initial cache for indices 0..5 (a(0) ... a(5)) */
    if (cache == NULL) {
        cache_cap = 6;                       /* indices 0..5 inclusive */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */
        cache[0] = 1;                        /* a(0) = 1 for the recurrence */
        for (size_t i = 1; i <= 5; ++i) {
            cache[i] = 1;                    /* a(1) .. a(5) = 1 */
        }
        max_computed = 5;
    }

    /* if requested index beyond current cache, enlarge it */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k <= 5) {     /* already initialized */
            continue;
        }

        idx_t R = k - 5;                /* upper limit of sum */
        seq_t sum = 0;
        seq_t C  = 1;                    /* C(R,0) */

        /* i = 0 term */
        sum += C * cache[0];

        /* iterate i = 1..R to compute C(R,i) successively */
        for (idx_t i = 1; i <= R; ++i) {
            /* C(R,i) = C(R,i-1) * (R - i + 1) / i   */
            C = C * (seq_t)(R - i + 1) / (seq_t)i;
            sum += C * cache[(size_t)i];
        }

        cache[(size_t)k] = sum;
    }

    max_computed = n;
    return cache[(size_t)n];
}
