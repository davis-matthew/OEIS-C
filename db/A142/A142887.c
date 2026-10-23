/*
 * A142887  Expansion of x*(5510*x^5-395*x^3-146*x^2-3*x-1) /
 *          (13340*x^4-52*x^2-1).
 * https://oeis.org/A142887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -52*a(n-2) + 13340*a(n-4);
   initial values: a(1)=1, a(2)=3, a(3)=94, a(4)=239 */

#include <stdlib.h>

/* multiply a seq_t value by a small unsigned integer using only + */
static seq_t mul_uint(seq_t x, uint32_t m) {
    seq_t res = 0;
    for (uint32_t i = 0; i < m; ++i)
        res += x;
    return res;
}

static seq_t *cache = NULL;        /* array of computed values 1..n */
static idx_t cache_cap = 0;        /* allocated capacity (no. of indices) */
static idx_t computed_max = 0;     /* highest n already computed */

/* Main function required by the framework */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety for n=0, not used normally */

    /* Initialise the cache on the first call */
    if (!cache) {
        cache_cap = (n > 6 ? n : 8);
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        /* initial conditions */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)94;
        cache[4] = (seq_t)239;
        cache[5] = (seq_t)8452;
        cache[6] = (seq_t)22082;
        computed_max = 6;
    }
    /* grow the cache if necessary */
    while (cache_cap < n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i < 7) continue;  /* values 1..6 are already set */
        seq_t term1 = mul_uint(cache[i - 4], 13340u);
        seq_t term2 = mul_uint(cache[i - 2], 52u);
        cache[i] = term1 - term2;
    }
    if (n > computed_max) computed_max = n;
    return cache[n];
}
