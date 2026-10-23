/*
 * A192254  0-sequence of reduction of (n^2) by x^2 -> x+1.
 * https://oeis.org/A192254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 5*a(n-3) + 5*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) - a(n-7) for n ≥ 8, with initial values a(1)=1, a(2)=1,
 *            a(3)=10, a(4)=26, a(5)=76, a(6)=184, a(7)=429. This recurrence
 *            follows from the empirical generating function G(x) =
 *            x*(1-3*x+9*x²-6*x³+2*x⁴)/((1-x)(1-x-x²)³)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed terms; indices 0..cache_cap are valid */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* highest index for which memory is allocated */
static idx_t cache_size = 0;  /* first index that has not yet been computed */

static const seq_t base[8] = { (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)10,
                               (seq_t)26, (seq_t)76, (seq_t)184, (seq_t)429 };

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for dependent code */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap   = 8;           /* capacity up to index 8 */
        cache       = (seq_t *)calloc(cache_cap + 1, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        for (idx_t i = 0; i < 8; ++i) cache[i] = base[i];
        cache_size = 8;            /* next index to compute */
    }

    /* grow cache to hold index n, if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        /* initialise newly allocated part to zero */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 8) {
            cache[i] = base[i];    /* only for safety; normally i>=8 here */
        } else {
            /* pos  = 4*a(i-1) + 5*a(i-4) + 3*a(i-5) */
            /* neg  = 3*a(i-2) + 5*a(i-3) + 2*a(i-6) + a(i-7) */
            seq_t pos = 4 * cache[i-1] + 5 * cache[i-4] + 3 * cache[i-5];
            seq_t neg = 3 * cache[i-2] + 5 * cache[i-3] + 2 * cache[i-6] + cache[i-7];
            cache[i] = pos - neg;   /* pos ≥ neg for all n≥8 */
        }
    }
    cache_size = n + 1;            /* we now have values up to n */
    return cache[n];
}
