/*
 * A207083  Number of n X 3 0..1 arrays avoiding 0 0 0 horizontally and 0 0 1
 *          vertically.
 * https://oeis.org/A207083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 5*a(n-2) - 14*a(n-3) + 11*a(n-4) + 4*a(n-5) -
 *            a(n-6) Initial terms (offset 1): a(1)=7, a(2)=49, a(3)=241,
 *            a(4)=1117, a(5)=4891, a(6)=20953
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A207083.
   n is 1‑based. */
seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* last index stored in cache */

    /* initialize cache with first six terms */
    if (cache == NULL) {
        size_t init = 7;            /* indices 0..6, 0 unused */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        cache[1] = (seq_t)7;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)241;
        cache[4] = (seq_t)1117;
        cache[5] = (seq_t)4891;
        cache[6] = (seq_t)20953;
        cache_size = 6;
    }

    /* return if already computed */
    if (n <= cache_size) return cache[n];

    /* grow cache if necessary */
    size_t new_size = (size_t)n + 1;   /* need indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0; /* allocation failure */
    cache = tmp;

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        const seq_t a1 = cache[i - 1];
        const seq_t a2 = cache[i - 2];
        const seq_t a3 = cache[i - 3];
        const seq_t a4 = cache[i - 4];
        const seq_t a5 = cache[i - 5];
        const seq_t a6 = cache[i - 6];

        /* compute 6*a1, 5*a2, 14*a3, 11*a4, 4*a5 by repeated addition */
        seq_t six_a1 = a1;
        for (int k = 1; k < 6; ++k) six_a1 += a1;

        seq_t five_a2 = a2;
        for (int k = 1; k < 5; ++k) five_a2 += a2;

        seq_t fourteen_a3 = a3;
        for (int k = 1; k < 14; ++k) fourteen_a3 += a3;

        seq_t eleven_a4 = a4;
        for (int k = 1; k < 11; ++k) eleven_a4 += a4;

        seq_t four_a5 = a5;
        for (int k = 1; k < 4; ++k) four_a5 += a5;

        /* accumulate according to the recurrence */
        seq_t val = six_a1;
        val -= five_a2;
        val -= fourteen_a3;
        val += eleven_a4;
        val += four_a5;
        val -= a6;

        cache[i] = val;
    }

    cache_size = n;
    return cache[n];
}
