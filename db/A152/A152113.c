/*
 * A152113  A001333 with terms repeated.
 * https://oeis.org/A152113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-4) for n ≥ 5 with initial values a(1)=1,
 *            a(2)=1, a(3)=3, a(4)=3
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization.  Index 0 is unused; indices start at 1. */
    static seq_t *cache   = NULL;
    static idx_t max_calc = 0;          /* largest n that has been computed */

    /* Return cached value if already computed. */
    if (n <= max_calc)
        return cache[n];

    /* Allocate or grow cache to accommodate up to n. */
    idx_t new_max = n;
    if (cache == NULL) {
        cache = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* out‑of‑memory; unlikely in tests */
        cache[0] = (seq_t)0;           /* unused, but defined for safety */
    } else if (new_max > max_calc) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (new_max + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        cache = tmp;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_calc + 1; i <= new_max; ++i) {
        if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)1;
        else if (i == 3)
            cache[3] = (seq_t)3;
        else if (i == 4)
            cache[4] = (seq_t)3;
        else {
            /* a(i) = 2*a(i-2) + a(i-4) .  Use addition to avoid overflow
               on intermediate multiplication. */
            seq_t t = cache[i - 2] + cache[i - 2];
            cache[i] = t + cache[i - 4];
        }
    }

    max_calc = new_max;
    return cache[n];
}
