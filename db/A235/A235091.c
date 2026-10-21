/*
 * A235091  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 13*a(n-2) - 39*a(n-3) - 26*a(n-4) + 78*a(n-5)
 *            Initial values: a(1)=80, a(2)=256, a(3)=808, a(4)=2580,
 *            a(5)=8184
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache to memoize values between calls */
    static seq_t *cache = NULL;   /* array of terms, 0‑based */
    static idx_t cap   = 0;       /* capacity of the array */
    static idx_t sz    = 0;       /* number of elements currently stored */

    /* The initial values (type seq_t may be larger than 8 bits) */
    static const seq_t init_vals[5] = {80, 256, 808, 2580, 8184};

    /* Offset specification: a(1) is the first term for n ≥ 1 */
    if (n == 0)  /* undefined for n = 0, but handle gracefully */
        return 0;

    /* Ensure cache has at least the initial 5 terms */
    if (sz < 5) {
        /* Allocate minimal capacity and store initial values */
        if (!cache) {
            cap = 8;               /* start with some space */
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        }
        for (idx_t i = sz; i < 5; ++i)
            cache[i] = init_vals[i];
        sz = 5;
    }

    /* If the requested term is already cached, return it */
    if (n <= sz)
        return cache[n - 1];

    /* Grow cache if necessary */
    if (n > cap) {
        idx_t new_cap = cap ? cap * 2 : 8;
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = sz; i < n; ++i) {
        seq_t a1 = cache[i - 1]; /* a(i) = a(i-1) */
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];

        seq_t val = 3 * a1 + 13 * a2;   /* 3*a(i-1) + 13*a(i-2) */
        val -= 39 * a3;                 /* minus 39*a(i-3) */
        val -= 26 * a4;                 /* minus 26*a(i-4) */
        val += 78 * a5;                 /* plus 78*a(i-5) */

        cache[i] = val;
    }
    sz = n;
    return cache[n - 1];
}
