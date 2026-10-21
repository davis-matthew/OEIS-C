/*
 * A208859  Number of n X 3 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 59*a(n-1) - 603*a(n-2) + 2211*a(n-3)
 *                        - 2826*a(n-4) + 720*a(n-5)
 * with seeds a(1)=5, a(2)=127, a(3)=5796, a(4)=273049,
 * a(5)=12883280, a(6)=607924387. */

#include <stdlib.h>

static const seq_t base_vals[6] = {
    (seq_t)5,
    (seq_t)127,
    (seq_t)5796,
    (seq_t)273049,
    (seq_t)12883280,
    (seq_t)607924387
};

static seq_t *cache = NULL;       /* dynamic array holding computed terms   */
static idx_t  cache_cap = 0;      /* allocated capacity                      */
static idx_t  cache_last = 0;     /* highest index already computed (1‑based) */

static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort with zero (should never happen for n<10000) */
        return;
    }
    cache = tmp;
    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    if (n < 1) {
        return (seq_t)0;
    }

    ensure_capacity(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i - 1] = base_vals[i - 1];
        } else {
            cache[i - 1] =
                ((seq_t)59  ) * cache[i - 2] -
                ((seq_t)603 ) * cache[i - 3] +
                ((seq_t)2211) * cache[i - 4] -
                ((seq_t)2826) * cache[i - 5] +
                ((seq_t)720 ) * cache[i - 6];
        }
    }
    cache_last = n;
    return cache[n - 1];
}
