/*
 * A170544  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (recurrence):
 *   a(0) = 1
 *   for n ≥ 1
 *     a(n) = 5 * Σ_{k=1}^{min(46,n)} a(n–k)
 *            – 15 * a(n–47)           (if n ≥ 47)
 *            + p(n)
 *
 *   where p(0) = 1, p(47) = 1,
 *         p(n) = 2   for 1 ≤ n ≤ 46,
 *         p(n) = 0   for n > 47.
 *
 * The implementation caches values so calls in arbitrary order are
 * efficient.  It is O(n·46) time and uses O(n) memory.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache     = NULL;   /* array holding a(0)…a(max)   */
    static size_t cache_cap = 0;      /* allocated capacity           */
    static idx_t  next_computed = 0;  /* first index not yet computed */

    /* ensure enough capacity for a(n) */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < need) new_cap <<= 1;   /* grow by powers of two */

#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap]();      /* value‑init all elements */
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* OOM → terminate */
        cache = tmp;
#endif
        cache_cap = new_cap;
    }

    /* if value already known, return it */
    if (n < next_computed) return cache[n];

    /* compute missing values up to n */
    for (idx_t i = next_computed; i <= n; ++i) {
        seq_t sum = 0;
        idx_t kmax = (i < 46 ? i : 46);

        for (idx_t k = 1; k <= kmax; ++k) sum += cache[i - k];

        seq_t val = 5 * sum;

        if (i >= 47) val -= 15 * cache[i - 47];

        if (i == 0 || i == 47)          val += 1;
        else if (i <= 46)               val += 2;   /* 1 ≤ i ≤ 46 */
        /* else (i > 47)  →  p(i) = 0, nothing to add */

        cache[i] = val;
    }

    next_computed = n + 1;
    return cache[n];
}
