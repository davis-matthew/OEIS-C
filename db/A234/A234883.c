/*
 * A234883  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-2) - 37*a(n-4) + 21*a(n-6) for n ≥ 7 Initial
 *            values: a(1) = 80, a(2) = 272, a(3) = 856, a(4) = 2896, a(5) =
 *            9192, a(6) = 31064
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t sz = 0;   /* number of terms already computed */
    static size_t cap = 0;  /* allocated capacity */

    /* First call: allocate cache and initialise the first six terms. */
    if (cache == NULL) {
        sz = 6;
        cap = 6;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL) exit(1);      /* OOM protection */
        cache[0] = (seq_t)80;
        cache[1] = (seq_t)272;
        cache[2] = (seq_t)856;
        cache[3] = (seq_t)2896;
        cache[4] = (seq_t)9192;
        cache[5] = (seq_t)31064;
    }

    /* Return immediately if the value is already known. */
    if (n <= (idx_t)sz)
        return cache[n - 1];

    /* Ensure the buffer can hold n terms. */
    if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);        /* OOM protection */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute the missing terms using the recurrence. */
    for (size_t i = sz + 1; i <= (size_t)n; ++i) {
        /* i ≥ 7 because sz ≥ 6. */
        seq_t val = (seq_t)14 * cache[i - 3]      /* a(i−2) */
                  - (seq_t)37 * cache[i - 5]      /* a(i−4) */
                  + (seq_t)21 * cache[i - 7];     /* a(i−6) */
        cache[i - 1] = val;
    }
    sz = (size_t)n;
    return cache[n - 1];
}
