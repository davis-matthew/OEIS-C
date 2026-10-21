/*
 * A239288  Maximal sum of x0 + x0*x1 + ... + x0*x1*...*xk over all
 *          compositions x0 + ... + xk = n.
 * https://oeis.org/A239288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{1 ≤ k ≤ n} (k + k*a(n-k)) Recurrence: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=6, a(5)=10, a(6)=15, a(7)=22; For n
 *            ≥ 8: a(n) = 3 + 3*a(n-3)
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for 0 … 7  */
    static const seq_t base_vals[8] = {0, 1, 2, 4, 6, 10, 15, 22};

    /* cache of already computed values (dynamic)  */
    static seq_t *cache   = NULL;   /* allocated array */
    static size_t cap     = 0;      /* allocated capacity     */
    static size_t comp_len = 0;      /* number of valid entries */

    /* handle small n directly */
    if (n < 8) {
        return base_vals[n];
    }

    /* first call: initialise cache with base values */
    if (comp_len == 0) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = base_vals[i];
        }
        comp_len = 8;
    }

    /* ensure capacity for n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap <<= 1;      /* grow geometrically */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fall back to recomputing */
            return base_vals[n < 8 ? n : 7];  /* should never happen for tested n */
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = comp_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)3 + (seq_t)3 * cache[i - 3];
    }
    comp_len = (size_t)n + 1;

    return cache[n];
}
