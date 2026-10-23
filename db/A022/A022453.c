/*
 * A022453  Expansion of 1/((1-x)*(1-5*x)*(1-7*x)*(1-9*x)).
 * https://oeis.org/A022453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = expansion of 1/((1-x)*(1-5*x)*(1-7*x)*(1-9*x))
 *            Recurrence: a(0)=1, a(1)=22, a(2)=320, a(3)=3890
 *            a(n)=22*a(n-1)-164*a(n-2)+458*a(n-3)-315*a(n-4) (n>=4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute a(n) with memoization; works for any seq_t and idx_t */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    /* Highest index that has been computed in the cache */
    static idx_t max_computed = (idx_t)-1;

    /* ------- initialization (first call) ------- */
    if (cache == NULL) {
        /* Allocate space for at least the first four terms, or up to n+1 if n<3 */
        size_t init_len = (n > 3 ? (size_t)n + 1 : 4);
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        /* Basic terms */
        cache[0] = (seq_t)1;
        if (init_len > 1) cache[1] = (seq_t)22;
        if (init_len > 2) cache[2] = (seq_t)320;
        if (init_len > 3) cache[3] = (seq_t)3890;

        /* Mark that the first four terms are available.
           max_computed may be larger than initialized indices if n>=3. */
        max_computed = 3;
    }

    /* If the requested term is already in the cache, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Expand the cache if needed */
    if ((size_t)n + 1 > (size_t)max_computed + 1) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* reallocation failed */
        cache = tmp;
    }

    /* Compute missing terms up to n using the recurrence */
    idx_t start = max_computed + 1;
    if (start < 4) start = 4;           /* recurrence valid for n>=4 */

    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)22;
        seq_t t2 = cache[i - 2] * (seq_t)164;
        seq_t t3 = cache[i - 3] * (seq_t)458;
        seq_t t4 = cache[i - 4] * (seq_t)315;
        cache[i] = t1 - t2 + t3 - t4;
    }

    max_computed = n;
    return cache[n];
}
