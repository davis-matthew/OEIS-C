/*
 * A211528  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 9*a(n-3) + 10*a(n-4) + 2*a(n-5) -
 *            4*a(n-6)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=8, a(2)=18, a(3)=34, a(4)=64, a(5)=116, a(6)=214 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small n can be answered directly */
    if (n <= 6) {
        static const seq_t init[7] = {0, 8, 18, 34, 64, 116, 214};
        return init[(size_t)n];
    }

    /* Memoize computed values */
    static seq_t *cache = NULL;          /* array where cache[i] = a(i) */
    static size_t cap = 0;                /* current allocated size */
    static size_t computed_to = 0;        /* largest n for which cache[n] is valid */

    /* Ensure we have space for all indices up to n */
    size_t target = (size_t)n;
    if (target + 1 > cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache = tmp;
        cap = new_cap;

        /* Initialise the first few values if not already present */
        static const seq_t init[7] = {0, 8, 18, 34, 64, 116, 214};
        for (size_t i = 1; i <= 6 && i < cap; ++i) {
            cache[i] = init[i];
        }
    }

    /* Compute any missing intermediate terms using the recurrence */
    if (computed_to < 6) computed_to = 6;   /* we already have a(1..6) */
    for (size_t i = computed_to + 1; i <= target; ++i) {
        /* recurrence: a(i) = 4*a(i-1) - 2*a(i-2) - 9*a(i-3) + 10*a(i-4) + 2*a(i-5) - 4*a(i-6) */
        cache[i] = 4 * cache[i-1]
                 - 2 * cache[i-2]
                 - 9 * cache[i-3]
                 +10 * cache[i-4]
                 + 2 * cache[i-5]
                 - 4 * cache[i-6];
    }
    computed_to = target;

    return cache[target];
}
