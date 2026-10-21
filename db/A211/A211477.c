/*
 * A211477  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having two or three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 11*a(n-3) + 2*a(n-4) + 10*a(n-5) -
 *            4*a(n-6) for n ≥ 8 Initial values: a(1)=8, a(2)=18, a(3)=40,
 *            a(4)=82, a(5)=168, a(6)=332, a(7)=658
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _a_init[7] = {
    (seq_t)8,   (seq_t)18,  (seq_t)40,
    (seq_t)82,  (seq_t)168, (seq_t)332,
    (seq_t)658
};

seq_t a(idx_t n)
{
    if (n <= 7)
        return _a_init[n-1];

    static seq_t *cache = NULL;     /* 1‑based cache of computed terms */
    static size_t cache_len = 0;    /* allocated size of cache (≥ n+1) */
    static idx_t  generated = 0;    /* highest index stored */

    /* Allocate (or grow) the cache if necessary */
    if (cache_len < (size_t)(n + 1)) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();                 /* out of memory */
        cache = tmp;
        cache_len = new_len;
    }

    /* Initialise base values if this is the first time */
    if (generated < 7) {
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = _a_init[i-1];
        generated = 7;
    }

    /* Compute missing terms up to n */
    for (idx_t i = generated + 1; i <= n; ++i) {
        seq_t term = cache[i-1] * (seq_t)3
                   + cache[i-2] * (seq_t)2
                   - cache[i-3] * (seq_t)11
                   + cache[i-4] * (seq_t)2
                   + cache[i-5] * (seq_t)10
                   - cache[i-6] * (seq_t)4;
        cache[i] = term;
    }
    generated = n;

    return cache[n];
}
