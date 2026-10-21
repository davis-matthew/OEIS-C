/*
 * A214663  Number of permutations of 1..n for which the partial sums of
 *          signed displacements do not exceed 2.
 * https://oeis.org/A214663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) * with a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t   cap      = 0;
    static idx_t    max_n    = 0;
    static bool     init     = false;

    /* enlarge cache if necessary */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure (unlikely in tests) */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first four values once */
    if (!init) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        max_n    = 3;
        init     = true;
    }

    /* answer is already available */
    if (n <= max_n)
        return cache[n];

    /* compute the missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(i) = a(i-1)+a(i-2)+3*a(i-3)+a(i-4) */
        seq_t term = cache[i-1] + cache[i-2] + (cache[i-3] << 1) + cache[i-3] + cache[i-4];
        cache[i] = term;
    }
    max_n = n;
    return cache[n];
}
