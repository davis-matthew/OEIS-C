/*
 * A170586  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170586 – number of reduced words of length n
 *  Recurrence derived from the generating function
 *
 *      G(t) = (t^47 + 2 t^46 + … + 2 t + 1) /
 *             (1128 t^47 − 47 t^46 − … − 47 t + 1)
 *
 *  For n ≥ 0:
 *
 *      rhs(n) = 1          if n == 0 or n == 47
 *      rhs(n) = 2          if 1 ≤ n ≤ 46
 *      rhs(n) = 0          otherwise
 *
 *      a(n) = rhs(n) − 1128 * a(n−47) + 47 * Σ_{k=1}^{min(46,n)} a(n−k)
 *
 *  The sequence starts with a(0)=1, a(1)=49, a(2)=2352, … (see OEIS A170586).
 *
 *  The implementation uses memoisation; it grows the cache only as needed
 *  and is safe to be called in any order.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed values    */
    static idx_t cached_max = (idx_t)-1; /* last index that has been computed */

    /* Allocate or grow cache if required */
    if (cached_max == (idx_t)-1) {       /* first call: allocate space up to n */
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) abort();
    } else if (n > cached_max) {        /* extend to accommodate new n    */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing terms from (cached_max+1) up to n */
    idx_t start = (cached_max == (idx_t)-1) ? 0 : (cached_max + 1);

    for (idx_t i = start; i <= n; ++i) {
        seq_t rhs;
        if (i == 0 || i == 47)
            rhs = (seq_t)1;
        else if (i <= 46)
            rhs = (seq_t)2;
        else
            rhs = (seq_t)0;

        /* Sum of the previous up to 46 terms */
        seq_t sum = 0;
        idx_t up = (i < 46) ? i : 46;
        for (idx_t k = 1; k <= up; ++k)
            sum += cache[i - k];

        /* Term involving a(i-47) – only if i >= 47 */
        seq_t sub = 0;
        if (i >= 47)
            sub = (seq_t)1128 * cache[i - 47];

        /* Compute a(i) */
        cache[i] = rhs - sub + (seq_t)47 * sum;
    }

    cached_max = n;
    return cache[n];
}
