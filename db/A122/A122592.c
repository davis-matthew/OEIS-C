/*
 * A122592  a(n) = - a(n-1) + a(n-3) + (a(n-1) - a(n-2))^2 + (a(n-2) -
 *          a(n-3))^2.
 * https://oeis.org/A122592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = - a(n-1) + a(n-3) + (a(n-1)-a(n-2))^2 + (a(n-2)-a(n-3))^2
 *            Initial values: a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 13, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* highest index computed in cache */

    /* Trivial base cases */
    if (n <= 3) {
        return 1;
    }

    /* First allocation and initialisation of the cache */
    if (!cache) {
        cache = (seq_t *)realloc(NULL, (size_t)4 * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – unlikely */
        cache[0] = 0;                  /* unused – keeps indices 1‑based */
        cache[1] = cache[2] = cache[3] = 1;
        computed = 3;
    }

    /* If we need more terms, enlarge the cache and compute them */
    if (n > computed) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – unlikely */
        cache = tmp;

        /* Compute only the new terms (from computed+1 up to n) */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* absolute differences */
            seq_t d1 = cache[i-1] > cache[i-2] ? cache[i-1] - cache[i-2]
                                              : cache[i-2] - cache[i-1];
            seq_t d2 = cache[i-2] > cache[i-3] ? cache[i-2] - cache[i-3]
                                              : cache[i-3] - cache[i-2];

            /* squares of the differences */
            seq_t s1 = d1 * d1;
            seq_t s2 = d2 * d2;

            /* recurrence: a(i) = a(i-3) + s1 + s2 - a(i-1) */
            seq_t val = cache[i-3];
            val += s1;
            val += s2;
            val -= cache[i-1];
            cache[i] = val;
        }

        computed = n;
    }

    return cache[n];
}
